#include <string.h>
#include "Mat.hpp"
#include "memoryutils.h"
//#include "tinyCNN_generated.h"

using namespace std;
namespace TCNN {
	Mat::Mat(int dimsize) {
		mBuffer.dimensions = dimsize;
		mBuffer.type = halide_type_of<float>();
		mBuffer.device = 0;
		mBuffer.host = nullptr;
		mBuffer.dim = &mShape[0];
	}

	Mat::Mat(const Mat* mat, bool allocMemory) {
		auto buffer = mat->buffer();
		mBuffer.dimensions = buffer.dimensions;
		mBuffer.type = buffer.type;
		mBuffer.device = 0;
		mBuffer.host = nullptr;
		mBuffer.dim = &mShape[0];

		for (int i = 0; i < buffer.dimensions; ++i)
		{
			mBuffer.dim[i].extent = buffer.dim[i].extent;
		}
		Mat::setLinearLayout(this);
		
		
		if (allocMemory)
		{
			auto memorySize = size();
			if (memorySize > 0)
			{
				mallocMemory = true;
				mBuffer.host = (uint8_t*)MemoryAllocAlign(memorySize, ALIGN_DEFAULT);
			}
		}

	}

	Mat::~Mat()
	{
		if (mallocMemory)
		{
			if (nullptr != mBuffer.host)
			{
				MemoryFreeAlign(mBuffer.host);
			}
		}
	}

	Mat* Mat::createDevice(const std::vector<int>& shape, halide_type_t type)
	{
		auto shapeMat = new Mat((int)shape.size());
		for (int i = 0; i < shape.size(); ++i)
		{
			shapeMat->setLength(i, shape[i]);
		}

		shapeMat->buffer().type = type;
		Mat::setLinearLayout(shapeMat);
		return shapeMat;
	}

	Mat* Mat::create(const std::vector<int>& shape, halide_type_t type, void* data)
	{
		Mat shapeMat((int)shape.size());
		for (int i = 0; i < shape.size(); ++i)
		{
			shapeMat.setLength(i, shape[i]);
		}
		shapeMat.buffer().type = type;
		bool ownData = data == nullptr;

		auto result = new Mat(&shapeMat, ownData);
		if (nullptr != data)
		{
			result->buffer().host = (uint8_t*)data;
		}
		return result;
	}

	std::vector<int> Mat::shape() const
	{
		std::vector<int> result;
		for (int i = 0; i < mBuffer.dimensions; ++i)
		{
			result.push_back(mBuffer.dim[i].extent);
		}
		return result;
	}

	int Mat::size() const
	{
		auto dataSize = mBuffer.type.bytes();
		for (int i = 0; i < this->buffer().dimensions; ++i)
		{
			int currentDimSize = mBuffer.dim[i].extent;
			/*if (NC4HW4 && i ==1)
			{
				currentDimSize = ALIGN_UP4(currentDimSize);
			}*/
			dataSize *= currentDimSize;
		}
		return dataSize;
	}

	void Mat::setLinearLayout(Mat* mat)
	{
		auto& buffer = mat->buffer();
		int size = 1;
		for (int i = 0; i < buffer.dimensions; ++i)
		{
			auto index = buffer.dimensions - i - 1;
			auto extent = buffer.dim[index].extent;
			/*if (1 == index && NC4HW4) {
				extent = ROUND_UP(extent, 4);
			}*/
			buffer.dim[index].stride = size;
			size *= extent;
		}

	}

	

} //namespace TCNN

