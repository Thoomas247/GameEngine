#pragma once

#include <string>

#include "imgui/imgui.h"

#ifdef DEV_MODE
#define LOG_ERROR(x) Log::LogError(x);
#define LOG_WARN(x) Log::LogWarning(x);
#define LOG_INFO(x) Log::LogInfo(x);
#endif // DEV_MODE

namespace Log
{
	extern ImVector<char*> Items;

	void LogError(const std::string& error);
	void LogWarning(const std::string& warning);
	void LogInfo(const std::string& info);
}