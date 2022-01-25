#include "Log.h"

#include <iostream>

ImVector<char*> Log::Items;

char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }

void addLog(const char* fmt, ...) IM_FMTARGS(2)
{
	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	Log::Items.push_back(Strdup(buf));
}

void Log::LogError(const std::string& error)
{
	std::string line = "[error] " + error;
	addLog(line.c_str());
	std::cout << error << std::endl;
}

void Log::LogWarning(const std::string& warning)
{
	std::string line = "[warning] " + warning;
	addLog(line.c_str());
}

void Log::LogInfo(const std::string& info)
{
	std::string line = "[info] " + info;
	addLog(line.c_str());
}
