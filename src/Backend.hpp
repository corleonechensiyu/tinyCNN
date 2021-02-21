#ifndef BACKEND_HPP_
#define BACKEND_HPP_

#include "Mat.hpp"
#include "tinyCNN_generated.h"
#include "Execution.hpp"
#include "NoCopy.hpp"

#include <stdio.h>
#include <map>
#include <memory>
#include <vector>

namespace TCNN {
struct Op;
class Execution;
class BufferAllocator;

class Backend :public NoCopy{
public:
	
    enum StorageType {
        STATIC,
        DYNAMIC,
		DYNAMIC_SEPERATE
    };
public:
	Backend(int numberThread = 4, size_t flags = 0);

	 ~Backend();
public:
	bool onAcquireBuffer(const Mat* nativeMat, StorageType storageType);
	bool onReleaseBuffer(const Mat* nativeMat, StorageType storageType);
	bool onClearBuffer();
	void onCopyBuffer(const Mat* srcMat, const Mat* dstMat) const;
	/*std::pair<float, bool> onMeasure(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs,
		const TCNN::Op* op);*/
	Execution* onCreate(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs,
		const TCNN::Op* op);
	void onExecuteBegin() const;
	void onExecuteEnd() const;

public:
	class Creator {
	public:
		virtual Execution* onCreate(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs,
			const TCNN::Op* op, Backend* backend) const = 0;
	};
	static bool addCreator(OpType t, Creator* c);
	


	BufferAllocator* getBufferAllocator() const {
		return mDynamicAllocator.get();
	}

	static void initCreatorMap();

protected:
	bool allocBuffer(int size, Mat* dest, StorageType storageType);

private:
	std::shared_ptr<BufferAllocator> mStaticAllocator;
	std::shared_ptr<BufferAllocator> mDynamicAllocator;
	bool mCheckNAN = false;
	int mThreadNumber;
	static std::map<OpType, Backend::Creator*>* getCreatorMap();
	static std::map<OpType, Backend::Creator*>* gCreator;
};


class RuntimeCreator {

public:
	virtual ~RuntimeCreator() = default;
	virtual Backend* onCreate(int numThread, size_t flags) const = 0;

protected:
	RuntimeCreator() = default;
};

PUBLIC const RuntimeCreator* GetExtraRuntimeCreator(int type);
PUBLIC bool InsertExtraRuntimeCreator(int type, const RuntimeCreator* creator);
//PUBLIC bool CPUCopyBuffer(const Mat* srcTensor, const Mat* dstTensor);


#define REGISTER_CPU_OP_CREATOR(name, opType)     \
    void ___##name##__##opType##__() {            \
        static name _temp;\
        Backend::addCreator(opType, &_temp); \
    }




}//namespace TCNN


#endif // !BACKEND_HPP_

