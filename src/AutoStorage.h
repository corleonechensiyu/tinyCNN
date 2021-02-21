#ifndef AUTOSTORAGE_H_
#define AUTOSTORAGE_H_

#include <stdint.h>
#include <string.h>
#include "memoryutils.h"

namespace TCNN {
template<typename T>
class AutoStorage {
public:
    AutoStorage() {
        mSize = 0;
        mData = NULL;
    }

    AutoStorage(int size) {
        mData = (T*)MemoryAllocAlign(sizeof(T) * size, ALIGN_DEFAULT);
        mSize = size;
    }

    ~AutoStorage() {
        if (NULL != mData) {
            MemoryFreeAlign(mData);
        }
    }

    inline int size() const {
        return mSize;
    }

    void set(T* data, int size) {
        if (NULL != mData && mData != data) {
            MemoryFreeAlign(mData);
        }
        mData = data;
        mSize = size;
    }


    void reset(int size) {
        if (NULL != mData) {
            MemoryFreeAlign(mData);
        }
        mData = (T*)MemoryAllocAlign(sizeof(T) * size, ALIGN_DEFAULT);
        mSize = size;
    }


    void release() {
        if (NULL != mData) {
            MemoryFreeAlign(mData);
            mData = NULL;
            mSize = 0;
        }
    }

    void clear() {
        ::memset(mData, 0, mSize * sizeof(T));
    }

    T* get() const {
        return mData;
    }

private:
    T* mData = NULL;
    int mSize = 0;
};

} //namespace TCNN
#endif // !AUTOSTORAGE_H_
