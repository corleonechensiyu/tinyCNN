namespace TCNN {
	extern void ___SoftmaxCreator__OpType_Softmax__();
	extern void ___ReLUCreator__OpType_ReLU__();
	extern void ___PoolingCreator__OpType_Pooling__();
	extern void ___ConvolutionCreator__OpType_Convolution__();
	extern void ___InnerProductCreator__OpType_InnerProduct__();

	void registerCPUOps() {
		___SoftmaxCreator__OpType_Softmax__();
		___ReLUCreator__OpType_ReLU__();
		___PoolingCreator__OpType_Pooling__();
		___ConvolutionCreator__OpType_Convolution__();
		___InnerProductCreator__OpType_InnerProduct__();
	}
}//namespace TCNN