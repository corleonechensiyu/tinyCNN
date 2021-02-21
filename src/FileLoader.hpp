#ifndef FILELOADER_HPP_
#define FILELOADER_HPP_
#include <vector>
#include "AutoStorage.h"

namespace TCNN {
	class PUBLIC FileLoader {
	public:
		FileLoader(const char* file);
		~FileLoader();
        bool read();

        bool valid() const {
            return mFile != nullptr;
        }
        inline size_t size() const {
            return mTotalSize;
        }

        bool merge(AutoStorage<uint8_t>& buffer);

    private:
        std::vector<std::pair<size_t, void*>> mBlocks;
        FILE* mFile = nullptr;
        static const int gCacheSize = 4096;
        size_t mTotalSize = 0;
	};


} //namespace TCNN



#endif // !FILELOADER_HPP_
