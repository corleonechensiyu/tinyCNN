#include "innerproduct.hpp"

namespace TCNN {
	InnerProductLayer::InnerProductLayer(Backend* b, const TCNN::Op* op):Execution(b)
	{
	}
	int InnerProductLayer::onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
	{
		return 0;
	}
	int InnerProductLayer::onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
	{
		return 0;
	}

	class InnerProductCreator :public Backend::Creator {
	public:
		virtual Execution* onCreate(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs,
			const TCNN::Op* op, Backend* backend) const override {
			return new InnerProductLayer(backend, op);
		}
	};

	REGISTER_CPU_OP_CREATOR(InnerProductCreator, OpType_InnerProduct);
}//namespace TCNN


