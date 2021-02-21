#ifndef EXCUTION_HPP_
#define EXCUTION_HPP_

#include "Mat.hpp"
#include "NoCopy.hpp"
#include <memory>
#include <string>
namespace TCNN {
class Backend;
struct Op;

class Execution :public NoCopy{
public:
	Execution() = delete;
	Execution(Backend* backend) : mBackEnd(backend) {}

	virtual ~Execution() = default;

	virtual int onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) {
		return 0;
	}

	virtual int onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) = 0;

public:
	//not useed???
	class Creator :public NoCopy {
	public:
		virtual ~Creator() = default;
		virtual Execution* onCreate(Backend* backend, const Op* op) const = 0;
	};

public:
	inline bool valid() const {
		return mValid;
	}

	Backend* backend() const {
		return mBackEnd;
	}

protected:
	bool mValid = true;

private:
	Backend* mBackEnd;
};


}//namespace TCNN


#endif // !EXCUTION_HPP_
