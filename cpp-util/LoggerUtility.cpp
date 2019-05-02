#include "LoggerUtility.h"
#include <thread>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace common;
using namespace std;
Logger::Logger()
{
	char file[255] = { 0 };
	sprintf_s(file, "log\\system_%s.log",getCurrentTime(true).c_str());
	fopen_s(&mSystemFile,file,"w");

	sprintf_s(file,"log\\algorithm_%s.log", getCurrentTime(true).c_str());
	fopen_s(&mAlgFile,file, "w");
	onLogThread = true;
	mFiles[0] = mSystemFile;
	mFiles[1] = mAlgFile;
	mQ = new std::queue<LogStruck>;
	std::thread tLog(&Logger::logThread,mQ);
	tLog.detach();
}

Logger::~Logger()
{
	onLogThread = false;
	if (mSystemFile)
	{
		fclose(mSystemFile);
	}
	if (mAlgFile)
	{
		fclose(mAlgFile);
	}

}


std::string Logger::getCurrentTime(bool isFile)
{
	char now[255] = { 0 };
#ifdef _WIN32
	SYSTEMTIME st;
	GetLocalTime(&st);
	
	if (isFile)
	{
		sprintf_s(now, "%02d_%02d_%02d_%02d_%02d_%02d", st.wYear,
			st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	}
	else
	{
		sprintf_s(now, "%02d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear,
			st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	}
#endif
	return std::string(now);
}

void Logger::buildLogInfo(const char* src, char* dest)
{
	sprintf_s(dest,256, "%s %s\n",getCurrentTime().c_str(),src);
}

void Logger::log(LogLevel ll, LogType lt, const char* info)
{
	LogStruck pushStruck;
	char dest[1000] = { 0 };
	buildLogInfo(info, dest);
	string str=dest;
	Mmutex.lock();
	pushStruck.mStr = str;
	pushStruck.mLl = ll;
	pushStruck.mLt = lt;
	mQ->push(pushStruck);
	Mmutex.unlock();
}

void Logger::logThread(std::queue<LogStruck> *mq)
{

	Logger* logger = Logger::instance();
	LogStruck	popStruck;
	while (logger->onLogThread)
	{

		if (!mq->empty())
		{
			logger->Mmutex.lock();
			popStruck = mq->front();
			std::string str;
			char dest[1024];
			LogType lt;
			LogLevel ll;
			str = popStruck.mStr;
			strcpy_s(dest, str.c_str());
			lt = popStruck.mLt;
			ll = popStruck.mLl;
			mq->pop();
			logger->Mmutex.unlock();
			if (logger->mFiles[lt])
			{
				fprintf(logger->mFiles[lt], dest);
				fflush(logger->mFiles[lt]);
			}
			if (logger->mCallback)
			{
				logger->mCallback(ll, lt, dest, logger->mContex);
			}
		}
		Sleep(1);
	}
	

}

void Logger::logD(LogType lt, const char* info)
{
	if (mLogLevel <= LL_DEBUG)
	{
		log(LL_DEBUG, lt, info);
	}
}

void Logger::logI(LogType lt, const char* info)
{
	if (mLogLevel <= LL_INFO)
	{
		log(LL_INFO, lt, info);
	}
}

void Logger::logE(LogType lt, const char* info)
{
	if (mLogLevel <= LL_INFO)
	{
		log(LL_ERROR, lt, info);
	}
}

void Logger::formatLogD(LogType lt, const char* format, ...)
{
	Logger* logger = Logger::instance();
	va_list args;
	va_start(args, format);
	char info[1024];
	vsnprintf(info, 1024, format, args);
	va_end(args);
	logger->logD(lt, info);
}

void Logger::formatLogE(LogType lt, const char* format, ...)
{
	Logger* logger = Logger::instance();
	va_list args;
	va_start(args, format);
	char info[1024];
	vsnprintf(info, 1024, format, args);
	logger->logE(lt, info);
	va_end(args);
}

void Logger::formatLogI(LogType lt, const char* format, ...)
{
	Logger* logger = Logger::instance();
	va_list args;
	va_start(args, format);
	char info[1024];
	vsnprintf(info, 1024, format, args);
	logger->logI(lt, info);
	va_end(args);
}

void Logger::setLevel(LogLevel ll)
{
	mLogLevel = ll;
}

void Logger::setCallBack(LogCallBack lcb, void* contex)
{
	mCallback = lcb;
	mContex = contex;
}