#include "precompiled.h"
#include "Util.h"

std::string Util::GetFileName(const std::string& path)
{
	size_t slashPos = path.find_last_of("/");
	size_t dotPos = path.find_last_of(".");
	if (slashPos == std::string::npos)
		slashPos = 0;
	else
		slashPos++;

	if (dotPos == std::string::npos || slashPos > dotPos)
	{
		LOG_ERROR("UTIL::File path invalid. It does not contain a file extension!");
	}

	return path.substr(slashPos, dotPos - slashPos);
}
