#ifndef CONVOLUTION_HPP_
#define CONVOLUTION_HPP_

#include "Backend.hpp"
namespace TCNN {
	class ConvolutionLayer :public Execution {
	public:
		ConvolutionLayer(Backend* b, const TCNN::Op *op);
		virtual ~ConvolutionLayer() = default;
		virtual int onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) override;
		virtual int onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) override;
	private:
		
		std::pair<int, std::function<void(int)>> mFunction;
	};



} //namespace TCNN

#endif // !CONVOLUTION_HPP_

