#ifndef POOLING_HPP_
#define POOLING_HPP_
#include "Backend.hpp"
namespace TCNN {
	class PoolingLayer : public Execution {
	public:
		PoolingLayer(Backend* b, const TCNN::Op *op);
		virtual ~PoolingLayer() = default;
		virtual int onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) override;
		virtual int onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) override;
	private:
		//const Pooling* mParameter;
		std::pair<int, std::function<void(int)>> mFunction;
};



} //namespace TCNN


#endif // !POOLING_HPP_
