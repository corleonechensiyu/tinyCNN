#include "convolution.hpp"

namespace TCNN {
    ConvolutionLayer::ConvolutionLayer(Backend* b, const TCNN::Op* op):TCNN::Execution(b)
    {
        //��Ҫ��һЩȷ���������Mat��shape
        //����weights��bais��Mat������onAcquireBuffer���������ڴ��С
		//�ٽ�ģ���е�Ȩ��copy��mweights��mbais��
    
    }

    int TCNN::ConvolutionLayer::onResize(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
    {
        //��������ڴ棬�ͷż����ڴ�
        //����backend()->onAcquireBuffer(&mCache, Backend::DYNAMIC)���л�������룬
        //����backend()->onReleaseBuffer(&mCache, Backend::DYNAMIC)���ջ��档�ͷź���ڴ���Ա�����
        return 0;
    }

    int TCNN::ConvolutionLayer::onExecute(const std::vector<Mat*>& inputs, const std::vector<Mat*>& outputs)
    {
        //����Ҫ������ļ�飬��������ǰ�������⡣��ִ�������ȷ����
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
