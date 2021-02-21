#include "convolution.hpp"

namespace TCNN {
    ConvolutionLayer::ConvolutionLayer(Backend* b, const TCNN::Op* op):TCNN::Execution(b)
    {
        //主要做一些确定输入输出Mat的shape
        //创建weights和bais的Mat及根据onAcquireBuffer函数分配内存大小
		//再将模型中的权重copy到mweights和mbais中
    
    }

    int TCNN::ConvolutionLayer::onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
    {
        //分配计算内存，释放计算内存
        //调用backend()->onAcquireBuffer(&mCache, Backend::DYNAMIC)进行缓存的申请，
        //调用backend()->onReleaseBuffer(&mCache, Backend::DYNAMIC)回收缓存。释放后的内存可以被复用
        return 0;
    }

    int TCNN::ConvolutionLayer::onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
    {
        //做必要的输入的检查，有利于提前发现问题。若执行完毕正确返回
        return 0;
    }

    class ConvolutionCreator : public Backend::Creator {
    public:
        virtual Execution* onCreate(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs,
            const TCNN::Op* op, Backend* backend) const override {
            return new ConvolutionLayer(backend, op);
        }
    };
    REGISTER_CPU_OP_CREATOR(ConvolutionCreator, OpType_Convolution);

} //namespace TCNN
