#pragma once

#include <vector>
#include <sstream>
using std::string;
using std::vector;

namespace common {

	struct Time
	{
		unsigned short year;
		unsigned char month;
		unsigned char day;
		unsigned char hour;
		unsigned char minute;
		unsigned char second;
		unsigned short millsecond;
	};

	 void split(const string &s, char delim,vector<string>& elems);

	 void currentTime(Time& t);
	 string time2String(Time& t);
}