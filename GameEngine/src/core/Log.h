#pragma once

#include <string>

#include "imgui/imgui.h"

#define LOG_ERROR(x) Log::LogError(x);
#define LOG_WARN(x) Log::LogWarning(x);
#define LOG_INFO(x) Log::LogInfo(x);

namespace Log
{
	extern ImVector<char*> Items;

	void LogError(const std::string& error);
	void LogWarning(const std::string& warning);
	void LogInfo(const std::string& info);
}