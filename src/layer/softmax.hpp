#ifndef SOFTMAX_HPP_
#define SOFTMAX_HPP_

#include "Backend.hpp"
namespace TCNN {
	class SoftmaxLayer : public Execution {
	public:
		SoftmaxLayer(Backend* b, const TCNN::Op* op);
		virtual ~SoftmaxLayer() = default;
		virtual int onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) override;
		virtual int onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) override;
	private:
		//const Softmax* mParameter;
		std::pair<int, std::function<void(int)>> mFunction;
	};
} //namespace TCNN


#endif // !SOFTMAX_HPP_
