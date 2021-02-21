#include "softmax.hpp"

namespace TCNN {
	SoftmaxLayer::SoftmaxLayer(Backend* b, const TCNN::Op* op) :Execution(b)
	{
	}
	int SoftmaxLayer::onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
	{
		return 0;
	}
	int SoftmaxLayer::onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
	{
		return 0;
	}

	class SoftmaxCreator :public Backend::Creator {
	public:
		virtual Execution* onCreate(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs,
			const TCNN::Op* op, Backend* backend) const override {
			return new SoftmaxLayer(backend, op);
		}
	};

	REGISTER_CPU_OP_CREATOR(SoftmaxCreator, OpType_Softmax);
} //namespace TCNN


