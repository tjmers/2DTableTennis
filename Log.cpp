#include "Log.h"

Log debugLog("debug");

void Log::updateTime() {
	now = time(0);
	localtime_s(&ltm, &now);
}

Log::Log(const char *logName) {
	updateTime();

	strcpy_s(fileName, logName);
	strcat_s(fileName, ("_log-" + std::to_string(ltm.tm_year + 1900) + "-" + std::to_string(ltm.tm_mon + 1) + "-" + std::to_string(ltm.tm_mday) + ".log").c_str());
	
	append("Log created...");
}

Log::~Log() {
	std::remove(fileName);
}

void Log::append(const std::string msg) {
	updateTime();

	outputStream.open(fileName, std::ios_base::app);
	if (ltm.tm_hour / 10 == 0) outputStream << '0';
	outputStream << ltm.tm_hour << ':';
	if (ltm.tm_min / 10 == 0) outputStream << '0';
	outputStream << ltm.tm_min << ':';
	if (ltm.tm_sec / 10 == 0) outputStream << '0';
	outputStream << ltm.tm_sec << "   ";
	outputStream << msg << '\n';
	outputStream.close();
}

void Log::warning(const std::string warn) {
	append("WARNING: " + warn);
}

void Log::error(const std::string err) {
	append("ERROR: " + err);
}
