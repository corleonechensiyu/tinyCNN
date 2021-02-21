#ifndef INNERPRODUCT_HPP_
#define INNERPRODUCT_HPP_
#include "Backend.hpp"
namespace TCNN {
	class InnerProductLayer : public Execution {
	public:
		InnerProductLayer(Backend* b, const TCNN::Op *op);
		virtual ~InnerProductLayer() = default;
		virtual int onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) override;
		virtual int onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs) override;
	private:
		//const InnerProduct* mParameter;
		std::pair<int, std::function<void(int)>> mFunction;
	};


} //namespace TCNN


#endif // !INNERPRODUCT_HPP_
