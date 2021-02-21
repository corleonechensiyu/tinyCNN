#ifndef INTERPRETER_HPP_
#define INTERPRETER_HPP_

#include <functional>
#include <map>
#include <memory>
#include <string>
#include "Mat.hpp"
namespace TCNN {
	struct ScheduleConfig
	{
		std::vector<std::string> saveMats;
		int numThread = 4;
		struct Path
		{
			std::vector<std::string> inputs;
			std::vector<std::string> outpus;
		};
		Path path;
		
	};
	class Session;
	struct Content;
	class Mat;
	class Backend;

	class PUBLIC Interpreter {
	public:
		static Interpreter* createFromFile(const char* file);
		static Interpreter* createFromBuffer(const void* buffer, size_t size);
		~Interpreter();

	public:
		Session* createSession(const ScheduleConfig& config);
		Session* createMultiPathSession(const std::vector<ScheduleConfig>& configs);

		bool releaseSession(Session* session);

		void resizeSession(Session* session);
		void releaseModel();

		//std::pair<const void*, size_t> getModelBuffer() const;
		int runSession(Session* session) const;

		Mat* getSessionInput(const Session* session, const char* name);
		Mat* getSessionOutput(const Session* session, const char* name);

		//const std::map<std::string, Mat*>& getSessionOutputAll(const Session* session) const;
		//const std::map<std::string, Mat*>& getSessionInputAll(const Session* session) const;

	public:
		void resizeTensor(Mat* tensor, const std::vector<int>& dims);
		void resizeTensor(Mat* tensor, int batch, int channel, int height, int width);
		//const Backend* getBackend(const Session* session, const Mat* tensor) const;

	private:
		static Interpreter* createFromBufferInternal(Content* net);
		Content* mNet = nullptr;
		Interpreter(Content* net);

		Interpreter(const Interpreter&) = delete;
		Interpreter(const Interpreter&&) = delete;
		Interpreter& operator=(const Interpreter&) = delete;
		Interpreter& operator=(const Interpreter&&) = delete;
	};

} //namespace TCNN

#endif // !INTERPRETER_HPP_
