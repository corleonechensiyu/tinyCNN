#ifndef BUFFERALLOCATOR_HPP_
#define BUFFERALLOCATOR_HPP_
#include <map>
#include <memory>
#include <vector>
#include "memoryutils.h"
#include "NoCopy.hpp"
namespace TCNN {

class PUBLIC BufferAllocator :public NoCopy {
public:
	class Allocator {
	public:
		Allocator() = default;
		virtual ~Allocator() = default;
		virtual std::pair<void*, int> onAlloc(int size) = 0;
		virtual void onRelease(std::pair<void*, int> ptr) = 0;
		static std::shared_ptr<Allocator> createDefault();
		static std::shared_ptr<Allocator> createRecurse(BufferAllocator * parent);
	};

	BufferAllocator(std::shared_ptr<Allocator> parent, int align =ALIGN_DEFAULT) : mAllocator(parent), mAlign(align) {
		// nothing to do
	}
	~BufferAllocator() {
		release();
	}


public:
	std::pair<void*, int> alloc(int size, bool seperate = false);
	bool free(std::pair<void*, int> pointer);
	void release(bool allRelease = true);
	size_t totalSize() const {
		return mTotalSize;
	}

	void barrierBegin();
	void barrierEnd();
	void beginGroup();
	void endGroup();

private:
	class Node {
	public:
		~Node();
		std::pair<void*, int> pointer;
		std::shared_ptr<Node> parent = nullptr;
		int32_t size;
		int16_t useCount = 0;
		Allocator* outside = nullptr;
	};

	typedef std::multimap<size_t, std::shared_ptr<Node>> FREELIST;

	static void returnMemory(FREELIST* list, std::shared_ptr<Node> node, bool permitMerge = true);
	std::pair<void*, int> getFromFreeList(FREELIST* list, int size, bool permiteSplit = true);

	std::map<std::pair<void*, int>, std::shared_ptr<Node>> mUsedList;
	FREELIST mFreeList;
	size_t mTotalSize = 0;

	FREELIST* mCurrentFreeList = nullptr;
	std::vector<std::shared_ptr<FREELIST>> mGroups;
	std::shared_ptr<Allocator> mAllocator;
	int mAlign;
};
} //namespace TCNN


#endif // !BUFFERALLOCATOR_HPP_

