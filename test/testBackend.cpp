#include "Mat.hpp"
#include "Backend.hpp"
#include "util.h"

#include <iostream>
int main()
{
    std::cout << "start" << std::endl;
    auto creator = TCNN::GetExtraRuntimeCreator(1);
    if (nullptr == creator)
    {
        PRINT("can't get creator!\n");
        return -1;
    }
    std::cout << "onCreate,TCNN::Mat::create " << std::endl;
    std::shared_ptr<TCNN::Backend> bn(creator->onCreate(1,0));
    std::shared_ptr<TCNN::Mat> hostTensor(TCNN::Mat::create<float>(std::vector<int>{1, 3, 224, 224}, nullptr));
	int elementSize = hostTensor->elementSize();
	auto hostData = hostTensor->host<float>();

	for (int i = 0; i < elementSize; i++)
	{
		int flagRandom = i % 255;
		hostData[i] = flagRandom;
	}
    std::cout << "TCNN::Mat::createDevice " << std::endl;
	std::shared_ptr<TCNN::Mat> deviceTensor_pre(TCNN::Mat::createDevice<float>(std::vector<int>{1, 3, 224, 224}));
	bn->onAcquireBuffer(deviceTensor_pre.get(), TCNN::Backend::STATIC);
	std::shared_ptr<TCNN::Mat> deviceTensor(TCNN::Mat::createDevice<float>(std::vector<int>{1, 3, 224, 224}));
	bn->onAcquireBuffer(deviceTensor.get(), TCNN::Backend::STATIC);
	bn->onCopyBuffer(hostTensor.get(), deviceTensor_pre.get());
	bn->onCopyBuffer(deviceTensor_pre.get(), deviceTensor.get());

	std::shared_ptr<TCNN::Mat> checkHostTensor(TCNN::Mat::create<float>(std::vector<int>{1, 3, 224, 224}, nullptr));
	bn->onCopyBuffer(deviceTensor.get(), checkHostTensor.get());
	auto backendCopyData = checkHostTensor->host<float>();

	for (int i = 0; i < elementSize; ++i) {
		if (backendCopyData[i] != hostData[i]) {
			PRINT("Error for NCHW Mid bn:%d, %f -> %f\n", i, hostData[i], backendCopyData[i]);
			return false;
		}
	}


    std::cout << "TCNN::Backend::DYNAMIC_SEPERATE " << std::endl;
	std::shared_ptr<TCNN::Mat> deviceTensor2(
		TCNN::Mat::createDevice<float>(std::vector<int>{1, 3, 224, 224}));
	bn->onAcquireBuffer(deviceTensor2.get(), TCNN::Backend::DYNAMIC_SEPERATE);
	bn->onCopyBuffer(hostTensor.get(), deviceTensor2.get());
	bn->onCopyBuffer(deviceTensor2.get(), checkHostTensor.get());
	for (int i = 0; i < elementSize; ++i) {
		if (backendCopyData[i] != hostData[i]) {
			PRINT("Error for NHWC Mid bn:%d, %f -> %f\n", i, hostData[i], backendCopyData[i]);
			return false;
		}
	}
    std::cout << "end " << std::endl;
    

    return 0;
}