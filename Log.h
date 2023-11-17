#pragma once

#include <ctime>
#include <fstream>
#include <string>

class Log {
	char fileName[100];
	std::ofstream outputStream;
	time_t now;
	tm ltm;

	void updateTime();
public:
	Log(const char *logName);
	~Log();

	void append(const std::string msg);
	void warning(const std::string warn);
	void error(const std::string err);
};

extern Log debugLog;
