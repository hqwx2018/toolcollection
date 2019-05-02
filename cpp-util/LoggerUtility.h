#pragma once
#include <string>
#include <queue>
#include <mutex>

namespace common
{
		enum LogLevel
		{
			LL_DEBUG,
			LL_INFO,
			LL_ERROR
		};

		enum LogType
		{
			LT_SYSTEM=10,
			LT_ALG,
			LT_COUNT
		};
		struct LogStruck
		{
			std::string mStr;
			LogLevel   mLl;
			LogType mLt;
		};
		typedef void(*LogCallBack)(LogLevel ll,LogType lt, const char* info,void* contex);
		class Logger
		{
			Logger();
		public:
			~Logger();


			static Logger* instance()
			{
				static Logger inst;
				return &inst;
			}

			void setLevel(LogLevel ll);
			void setCallBack(LogCallBack lcb, void* contex);
			void logD(LogType lt, const char* info);
			void logI(LogType lt, const char* info);
			void logE(LogType lt, const char* info);
			static void formatLogD(LogType lt, const char* format, ...);
			static void formatLogI(LogType lt, const char* format, ...);
			static void formatLogE(LogType lt, const char* format, ...);

			static void logThread(std::queue<LogStruck> *mq);
			//isFile为true时表示用于文件名
			static std::string getCurrentTime(bool isFile=false);
			
		protected:
			void buildLogInfo(const char* src,char* dest);
			void log(LogLevel ll, LogType lt, const char* info);
			FILE* mSystemFile;
			FILE* mAlgFile;
			FILE* mFiles[LT_COUNT];
			LogLevel mLogLevel;
			LogCallBack mCallback;
			void* mContex;
			bool onLogThread;
			std::mutex Mmutex;
			std::queue<LogStruck> *mQ;
		};
	}


