#include "FileLoader.hpp"
#if defined(_MSC_VER)
#include <Windows.h>
#endif // defined(_MSC_VER)
namespace TCNN {
	FileLoader::FileLoader(const char* file)
	{
#if defined(_MSC_VER)
        wchar_t wFilename[1024];
        if (0 == MultiByteToWideChar(65001 /* UTF8 */, 0, file, -1, wFilename, sizeof(wFilename))) {
            mFile = nullptr;
            return;
        }
#if _MSC_VER >= 1400
        if (0 != _wfopen_s(&mFile, wFilename, L"rb")) {
            mFile = nullptr;
            return;
        }
#else
        mFile = _wfopen(wFilename, L"rb");
#endif
#else
        mFile = fopen(file, "rb");
#endif
	}
    FileLoader::~FileLoader() {
        if (nullptr != mFile) {
            fclose(mFile);
        }
        for (auto iter : mBlocks) {
            MemoryFreeAlign(iter.second);
        }
    }

    bool FileLoader::read()
    {
        auto block = MemoryAllocAlign(gCacheSize, ALIGN_DEFAULT);
        if (nullptr == block)
        {
            PRINT("Memory Alloc Failed\n");
            return false;
        }
        auto size = fread(block, 1, gCacheSize, mFile);
        mTotalSize = size;
        mBlocks.push_back(std::make_pair(size, block));

        while (size == gCacheSize) {
            block = MemoryAllocAlign(gCacheSize, ALIGN_DEFAULT);
            if (nullptr == block) {
                PRINT("Memory Alloc Failed\n");
                return false;
            }
            size = fread(block, 1, gCacheSize, mFile);
            if (size > gCacheSize) {
                PRINT("Read file Error\n");
                MemoryFreeAlign(block);
                return false;
            }
            mTotalSize += size;
            mBlocks.push_back(std::make_pair(size, block));
        }
        if (ferror(mFile)) {
            return false;
        }
        return true;



    }

    bool FileLoader::merge(AutoStorage<uint8_t>& buffer)
    {
        buffer.reset((int)mTotalSize);
        if (buffer.get() == nullptr) {
            PRINT("Memory Alloc Failed\n");
            return false;
        }

        auto dst = buffer.get();
        int offset = 0;
        for (auto iter : mBlocks)
        {
            memcpy(dst + offset, iter.second, iter.first);
            offset += iter.first;
        }

        return true; 
    }




} //namespace TCNN
