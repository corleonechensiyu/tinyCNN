#ifndef RELU_HPP_
#define RELU_HPP_
#include "Backend.hpp"
namespace TCNN {
	class ReluLayer : public Execution {
	public:
		ReluLayer(Backend* b, const TCNN::Op *op);
		virtual ~ReluLayer() = default;
		virtual int onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) override;
		virtual int onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) override;
	private:
		//const ReLU* mParameter;
		std::pair<int, std::function<void(int)>> mFunction;
	};


} //namespace TCNN


#endif // !RELU_HPP_
