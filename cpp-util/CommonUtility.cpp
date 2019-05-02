#include "CommonUtility.h"
#include <windows.h>
#include <vector>
#include <string>

using namespace common;
void currentTime(Time& t)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	t.year = st.wYear;
	t.month = st.wMonth;
	t.day = st.wDay;
	t.hour = st.wHour;
	t.minute = st.wMinute;
	t.second = st.wSecond;
	t.millsecond = st.wMilliseconds;
}

std::string time2String(Time& t)
{
	return std::to_string(t.year) + "_" + std::to_string(t.month) + "_" + std::to_string(t.day) + "_" +
		std::to_string(t.hour) + "_" + std::to_string(t.minute) + "_" + std::to_string(t.second) + "_" + std::to_string(t.millsecond);
}

void common::split(const string & s, char delim, vector<string>& elems)
{
	elems.clear();
	std::stringstream ss(s);
	std::string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}
