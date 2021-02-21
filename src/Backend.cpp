#include "Backend.hpp"
#include "BufferAllocator.hpp"
#include "Mat.hpp"

#include <cmath>
#include <omp.h>
#include <mutex>
namespace TCNN {
void registerCPUOps();

std::map<OpType, Backend::Creator*>* Backend::gCreator = nullptr;

void Backend::initCreatorMap()
{
	gCreator = new std::map<OpType, Backend::Creator*>;
}

std::map<OpType, Backend::Creator*>* Backend::getCreatorMap()
{
	return gCreator;
}

bool Backend::addCreator(OpType t, Creator* c)
{
	auto map = getCreatorMap();
	if (map->find(t) != map->end())
	{
		PRINT("Error: %d type has be added\n", t);
		return false;
	}
	map->insert(std::make_pair(t, c));
	return true;
}

Backend::Backend(int numberThread, size_t flags) : mThreadNumber(numberThread)
{
	mCheckNAN = flags == 1;
	mStaticAllocator.reset(new BufferAllocator(BufferAllocator::Allocator::createDefault()));
	std::shared_ptr<BufferAllocator::Allocator> defaultAlloc(BufferAllocator::Allocator::createRecurse(mStaticAllocator.get()));
	mDynamicAllocator.reset(new BufferAllocator(defaultAlloc));
}

Backend::~Backend()
{

}

void Backend::onExecuteBegin() const
{
	omp_set_dynamic(0);
	omp_set_num_threads(mThreadNumber);
}

void Backend::onExecuteEnd() const
{

}


bool Backend::allocBuffer(int size, Mat* dest, StorageType storageType)
{
	if (size <=0)
	{
		return false;
	}
	if (size > 1024 *1024 *500)
	{
		PRINT("Size larger than 500 M :%d\n", size);
	}

	auto& buffer = dest->buffer();
	std::pair<void*, int> points;
	switch (storageType) {
	case STATIC: {
		points = mStaticAllocator->alloc(size, false);
		break;
	}
	case DYNAMIC: {
		points = mDynamicAllocator->alloc(size, false);
		break;
	}
	case DYNAMIC_SEPERATE: {
		points = mDynamicAllocator->alloc(size, true);
		break;
	}
	default:
		ASSERT(false);
		break;
	}

	if (nullptr == points.first) {
		ERROR("Alloc buffer error for cpu backend\n");
		return false;
	}
	buffer.host = (uint8_t*)points.first + points.second;
	buffer.offset = points.second;
	if (buffer.type.code ==halide_type_handle)
	{
		memset(buffer.host, 0, size);
		buffer.offset = 0;
	}

	return true;
}

bool Backend::onAcquireBuffer(const Mat* nativeMat, StorageType storageType)
{
	if (nullptr == nativeMat)
	{
		return false;
	}
	auto nativeTensor = (Mat*)nativeMat;
	auto size = nativeTensor->size();

	return allocBuffer(size, nativeTensor, storageType);
}

bool Backend::onReleaseBuffer(const Mat* nativeMat, StorageType storageType)
{
	if (DYNAMIC_SEPERATE == storageType)
	{
		return true;
	}

	if (nativeMat ==nullptr)
	{
		return false;
	}

	if (nullptr == nativeMat->buffer().host)
	{
		return false;
	}

	std::pair<void*, int> pointer;
	pointer.second = nativeMat->buffer().offset;
	pointer.first = (uint8_t*)nativeMat->buffer().host - nativeMat->buffer().offset;

	if (STATIC == storageType)
	{
		mStaticAllocator->free(pointer);
		return true;
	}
	mDynamicAllocator->free(pointer);
	return true;
}

Execution* Backend::onCreate(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs,
	const TCNN::Op* op)
{
	auto map = getCreatorMap();
	auto iter = map->find(op->type());
	if (iter == map->end())
	{
		PRINT("Don't support type [%s], %s\n", TCNN::EnumNameOpType(op->type()), op->name()->c_str());
		return nullptr;
	}
	auto exe = iter->second->onCreate(inputs, outputs, op, this);
	if (nullptr == exe)
	{
		PRINT("The Creator Don't support type [%s], %s\n", TCNN::EnumNameOpType(op->type()), op->name()->c_str());
		return nullptr;
	}
	//TODO remove this in future
	if(mCheckNAN)
	{
		class CheckNANExecution : public Execution {
		public:
			CheckNANExecution(Execution* exe) : Execution(exe->backend()) {
				mExecution.reset(exe);
				mValid = exe->valid();
			}
			virtual ~CheckNANExecution() {
				// Do nothing
			}
			virtual int onResize(const std::vector<Mat*>& inputs,
				const std::vector<Mat*>& outputs) override {
				return mExecution->onResize(inputs, outputs);
			}

			virtual int onExecute(const std::vector<Mat*>& inputs,
				const std::vector<Mat*>& outputs) override {
				for (auto tensor : inputs) {
					if (halide_type_float != tensor->getType().code) {
						return 0;
					}
					auto size = tensor->elementSize();
					auto ptr = tensor->host<float>();
					for (int i = 0; i < size; ++i) {
						auto value = ptr[i];
						if (std::isnan(value) || std::isinf(value)) {
							return -1;
						}
					}
				}
				auto code = mExecution->onExecute(inputs, outputs);
				if (0 != code) {
					return code;
				}
				for (auto tensor : outputs) {
					if (halide_type_float != tensor->getType().code) {
						return 0;
					}
					auto size = tensor->elementSize();
					auto ptr = tensor->host<float>();
					for (int i = 0; i < size; ++i) {
						auto value = ptr[i];
						if (std::isnan(value) || std::isinf(value)) {
							return -1;
						}
					}
				}
				return 0;
			}

		private:
			std::unique_ptr<Execution> mExecution;
		};
		return new CheckNANExecution(exe);
	}
	return exe;


}

bool Backend::onClearBuffer()
{
	mDynamicAllocator->release(true);
	return true;
}

void Backend::onCopyBuffer(const Mat* srcMat, const Mat* dstMat) const
{
	auto ib = srcMat->buffer();
	auto ob = dstMat->buffer();
	memcpy(ob.host, ib.host, srcMat->size());
}

class CPURuntimeCreator : public RuntimeCreator {
public:
	virtual Backend* onCreate(int numThread, size_t flags) const override {
		return new Backend(numThread, flags);
	}
};


void registerCPURuntimeCreator()
{
	Backend::initCreatorMap();
	registerCPUOps();
	InsertExtraRuntimeCreator(1, new CPURuntimeCreator);
}


static std::once_flag s_flag;
void registerBackend()
{
	std::call_once(s_flag, [&]() {registerCPURuntimeCreator(); });
}


static std::once_flag gInitFlag;
static std::map<int, const RuntimeCreator*>& GetExtraCreator()
{
	static std::map<int, const RuntimeCreator*>* gExtraCreator;
	std::call_once(gInitFlag,
		[&]() { gExtraCreator = new std::map<int, const RuntimeCreator*>; });
	return *gExtraCreator;
}

const RuntimeCreator* GetExtraRuntimeCreator(int type)
{
	registerBackend();
	auto& gExtraCreator = GetExtraCreator();
	auto iter = gExtraCreator.find(type);
	if (iter == gExtraCreator.end()) {
		return nullptr;
	}

	return iter->second;
	
}

bool InsertExtraRuntimeCreator(int type, const RuntimeCreator* creator)
{
	auto& gExtraCreator = GetExtraCreator();
	if (gExtraCreator.find(type) != gExtraCreator.end())
	{
		return false;
	}
	gExtraCreator.insert(std::make_pair(type, creator));
	return true;

}

} //namespace TCNN