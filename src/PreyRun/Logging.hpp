#pragma once

#include "PreyRun.hpp"
#include "StdLib.hpp"


namespace pr
{
	void Log(const char* fmt, ...);
	void DebugLog(const char* fmt, ...);
	void FunctionLog(const char* func, const char* fmt, ...);
	void Warning(const char* fmt, ...);
}