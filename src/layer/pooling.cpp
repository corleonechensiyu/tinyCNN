#include "pooling.hpp"

namespace TCNN {
	PoolingLayer::PoolingLayer(Backend* b, const TCNN::Op* op) : Execution(b)
	{
	}
	int PoolingLayer::onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
	{
		return 0;
	}
	int PoolingLayer::onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
	{
		return 0;
	}

	class PoolingCreator : public Backend::Creator {
	public:
		virtual Execution* onCreate(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs,
			const TCNN::Op* op, Backend* backend) const override {
			return new PoolingLayer(backend, op);
		}
	};
	REGISTER_CPU_OP_CREATOR(PoolingCreator, OpType_Pooling);
}//namespace TCNN

