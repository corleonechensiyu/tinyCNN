#include "relu.hpp"

namespace TCNN {
	ReluLayer::ReluLayer(Backend* b, const TCNN::Op* op) :TCNN::Execution(b)
	{
	}

	int ReluLayer::onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
	{
		return 0;
	}

	int ReluLayer::onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
	{
		return 0;
	}


	class ReLUCreator :public Backend::Creator {
	public:
		virtual Execution* onCreate(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs,
			const TCNN::Op* op, Backend* backend) const override {
			return new ReluLayer(backend, op);
		}
	};

	REGISTER_CPU_OP_CREATOR(ReLUCreator, OpType_ReLU);
} //namespace TCNN


