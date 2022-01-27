#pragma once

#include <string>

#include "imgui/imgui.h"

#define LOG_ERROR(x) Log::LogError(x);
#define LOG_WARN(x) Log::LogWarning(x);
#define LOG_INFO(x) Log::LogInfo(x);

class Log
{
private:
	static ImVector<char*> s_Items;

public:
	static void LogError(const std::string& error);
	static void LogWarning(const std::string& warning);
	static void LogInfo(const std::string& info);

	static ImVector<char*> GetLog() { return s_Items; }

private:
	static void addLog(const char* fmt, ...) IM_FMTARGS(2);
};