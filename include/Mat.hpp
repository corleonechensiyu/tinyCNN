#ifndef MAT_HPP_
#define MAT_HPP_

#include <vector>
#include "HalideRuntime.h"
#include "util.h"

namespace TCNN {

class Backend;
class PUBLIC Mat {
public:
	

	Mat(int dimsize = 4);
	Mat(const Mat* mat, bool allocMemory = true);
	~Mat();

private:
	Mat(const Mat& mat) = delete;
	Mat(const Mat&& mat) = delete;
	Mat& operator=(const Mat&) = delete;
	Mat& operator=(const Mat&&) = delete;
public:
	static Mat* createDevice(const std::vector<int>& shape, halide_type_t type);

	template<typename T>
	static Mat* createDevice(const std::vector<int>& shape) {
		return createDevice(shape, halide_type_of<T>());
	}

	static Mat* create(const std::vector<int>& shape, halide_type_t type, void* data = NULL);

	template<typename T>
	static Mat* create(const std::vector<int>& shape, void* data = NULL) {
		return create(shape, halide_type_of<T>(), data);
	}

	//bool copyToHostMat(Mat* hostMat) const;

	//static InsideDescribe* getDescribe(const Mat* mat);
	static void setLinearLayout(Mat* mat);
	//static void clearHandleData(Mat* mat);

public:
	const halide_buffer_t& buffer() const {
		return mBuffer;
	}

	halide_buffer_t& buffer() {
		return mBuffer;
	}

	//void setType(int type);

	inline halide_type_t getType() const {
		return mBuffer.type;
	}

	template <typename T>
	T* host() const {
		return (T*)mBuffer.host;
	}

	uint64_t deviceId() const {
		return mBuffer.device;
	}
public:
	int dimensions() const {
		return mBuffer.dimensions;
	}

	std::vector<int> shape() const;

	int size() const;

	inline int elementSize() const {
		return size() / mBuffer.type.bytes();
	}

public:
	inline int width() const {
		return mBuffer.dim[3].extent;
	}

	inline int height() const {
		return mBuffer.dim[2].extent;
	}

	inline int channel() const {
		return mBuffer.dim[1].extent;
	}

	inline int batch() const {
		return mBuffer.dim[0].extent;
	}

	inline int stride(int index) const {
		return mBuffer.dim[index].stride;
	}
	inline int length(int index) const {
		return mBuffer.dim[index].extent;
	}
	inline void setStride(int index, int stride) {
		mBuffer.dim[index].stride = stride;
	}
	inline void setLength(int index, int length) {
		mBuffer.dim[index].extent = length;
	}

private:
	halide_buffer_t mBuffer;
	halide_dimension_t mShape[4];
	Backend* mbackend = nullptr;
	bool mallocMemory = false;
	
	//struct InsideDescribe* mDescribe;

};
}

#endif // !MAT_HPP_

