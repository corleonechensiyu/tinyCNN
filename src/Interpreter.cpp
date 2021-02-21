#include "Interpreter.hpp"
#include "tinyCNN_generated.h"
#include "AutoStorage.h"
#include "FileLoader.hpp"

#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <mutex>
#include <vector>

namespace TCNN {
	struct Content
	{
		AutoStorage<uint8_t> buffer;
		const Net* net = nullptr;
		std::vector<std::unique_ptr<Session>> sessions;
		std::map<const Mat*, const Session*> tensorMap;
		std::mutex lock;
	};



	Interpreter* Interpreter::createFromFile(const char* file)
	{
		if (nullptr == file) {
			PRINT("NULL file for create interpreter\n");
			return nullptr;
		}
		std::unique_ptr<FileLoader> loader(new FileLoader(file));

		if (!loader ->valid())
		{
			PRINT("Create interpreter failed, open %s error\n", file);
			return nullptr;
		}
		bool result = loader->read();

		if (!result) {
			PRINT("Read file error\n");
			return nullptr;
		}

		if (loader->size() == 0) {
			PRINT("Create interpreter failed, %s is empty\n", file);
			return nullptr;
		}

		auto net = new Content;
		bool success = loader->merge(net->buffer);
		if (!success) {
			return nullptr;
		}
		loader.reset();
		return createFromBufferInternal(net);
	}

	Interpreter* Interpreter::createFromBuffer(const void* buffer, size_t size)
	{
		if (nullptr == buffer || 0 == size) {
			PRINT("Buffer is null for create interpreter\n");
			return nullptr;
		}
		auto net = new Content;
		net->buffer.reset((int)size);
		if (nullptr == net->buffer.get())
		{
			ERROR("Memory not enought!\n");
			return nullptr;
		}
		memcpy(net->buffer.get(), buffer, size);
		return createFromBufferInternal(net);
	}


	Interpreter* Interpreter::createFromBufferInternal(Content* net)
	{
		if (nullptr ==net)
		{
			PRINT("Buffer is null for create interpreter\n");
			return nullptr;
		}
		flatbuffers::Verifier verify((const uint8_t*)(net->buffer.get()), net->buffer.size());
		if (false == VerifyNetBuffer(verify)) {
			PRINT("Invalidate buffer to create interpreter\n");
			delete net;
			return nullptr;
		}
		net->net = GetNet(net->buffer.get());
		if (nullptr == net->net->oplists()) {
			ERROR("Model has no oplist\n");
			delete net;
			return nullptr;
		}
		int opSize = net->net->oplists()->size();
		for (int i = 0; i < opSize; ++i)
		{
			auto op = net->net->oplists()->GetAs<Op>(i);
			if (nullptr ==op || nullptr == op->outputIndexes()) //TODO ȥ��outputIndexes��������Ϊblobname
			{
				ERROR("Invalid Model, the %d op is empty\n", i);
				delete net;
				return nullptr;
			}
		}
		return new Interpreter(net);
	}

	Interpreter::Interpreter(Content* net)
	{
		ASSERT(nullptr != net);
		mNet = net;
	}

	Interpreter::~Interpreter()
	{
		{
			std::unique_lock<std::mutex> _l(mNet->lock);
			mNet->sessions.clear();
			mNet->tensorMap.clear();
		}
		delete mNet;
	}

	Session* Interpreter::createMultiPathSession(const std::vector<ScheduleConfig>& configs)
	{
		if (nullptr == mNet->buffer.get()) {
			ERROR("The model buffer has been released. Can't create session\n");
			return nullptr;
		}
		std::unique_lock<std::mutex> _l(mNet->lock);
		//ԭʼ��schedule����Ҫ���� 
		//1.��ʼ������Mat  _setUpTensorInfo
		//2.��ȡBackend   getApprociateType
		//3.�õ�����Input������op    generateScheduleGraph

		//4.�õ�pipelineinfo����Ҫ��Ϣ��ÿ�����������input�����output Mat   initPipelineInfosFromOps
		//5.�Զ��õ�����ģ�͵��������� ���������ǿ����ֶ������������   setInputOutputForOps
		//6. �м����Ҳ����Ϊoutput
		//7.��scheduleinfo�������������name��Mat
		//8.����ʼ����Mat���ӵ�scheduleinfo��

		//new session
		//9.����Session��Mat�ƶ��� ȷ��Backend
		//10.new pipeline

		//11.Session resize����  1. clearcache��������Mat��usecout���㣬backend����Ϊnullptr��
		//                       2.pipeline ��encode ������
		//						 2.1computesize������ÿ����·��shape��������OP���綨����ȷ��
		//						 3.pipeline��allocMemory�����������ڴ棬�ɸ���pipelineinfo�е�ÿ��Mat
		//							����usecout��һ���������ž���Execution��ͨ��Backend��onCreate��������
		//							ÿ��OP��onCreate����������weights��bais��Mat������onAcquireBuffer���������ڴ��С
		//							�ٽ�ģ���е�Ȩ��copy��mweights��mbais�У�����Execution
		//						 4.����������ͼ���ڴ���䣬
		//						 5.������ÿ��Op��Execution��onResize������
		//						 6.intput��Mat��usecout ��1���ڴ��ͷ�onReleaseBuffer

		return nullptr;
	}


	Session* Interpreter::createSession(const ScheduleConfig& config) {
    	return createMultiPathSession({config});
	}

	bool Interpreter::releaseSession(Session* session)
	{
		return false;
	}

	void Interpreter::resizeSession(Session* session)
	{
		return;
	}

	void Interpreter::releaseModel()
	{
		return ;
	}


	int Interpreter::runSession(Session* session) const
	{
		return 0;
	}

	Mat* Interpreter::getSessionInput(const Session* session, const char* name)
	{
		return nullptr;
	}

	Mat* Interpreter::getSessionOutput(const Session* session, const char* name)
	{
		return nullptr;
	}

	// const std::map<std::string, Mat*>& Interpreter::getSessionOutputAll(const Session* session) const
	// {
	// 	return std::map<std::string, Mat*>();
	// }

	// const std::map<std::string, Mat*>& Interpreter::getSessionInputAll(const Session* session) const
	// {
	// 	return std::map<std::string, Mat*>();
	// }

	void Interpreter::resizeTensor(Mat* tensor, const std::vector<int>& dims){
		return;
	}


	void Interpreter::resizeTensor(Mat* tensor, int batch, int channel, int height, int width)
	{
		return;
	}
	


}// namespace TCNN
