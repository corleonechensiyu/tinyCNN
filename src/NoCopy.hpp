#ifndef NOCOPY_HPP_
#define NOCOPY_HPP_
namespace TCNN {
	class NoCopy {
	public:
		NoCopy() = default;
		NoCopy(const NoCopy&) = delete;
		NoCopy(const NoCopy&&) = delete;
		NoCopy& operator=(const NoCopy&) = delete;
		NoCopy& operator=(const NoCopy&&) = delete;
	};
}//namespace TCNN

#endif // !NOCOPY_HPP_
