#include "../idLib/precompiled.h"
#pragma hdrstop

#include "Logging.hpp"
#include "../game/Game_local.h"

namespace pr
{
	void Log(const char* fmt, ...)
	{
		va_list		argptr;
		char		text[MAX_STRING_CHARS];

		va_start(argptr, fmt);
		idStr::vsnPrintf(text, sizeof(text), fmt, argptr);
		va_end(argptr);

		common->Printf("PreyRun: %s\n", text);
	}

	void DebugLog(const char* fmt, ...)
	{
#ifdef PR_DEBUG
		va_list		argptr;
		char		text[MAX_STRING_CHARS];

		va_start(argptr, fmt);
		idStr::vsnPrintf(text, sizeof(text), fmt, argptr);
		va_end(argptr);

		common->Printf("PreyRun DBG: %s\n", text);
#endif // PR_DEBUG
	}

	void FunctionLog(const char* func, const char* fmt, ...)
	{
		va_list		argptr;
		char		text[MAX_STRING_CHARS];

		va_start(argptr, fmt);
		idStr::vsnPrintf(text, sizeof(text), fmt, argptr);
		va_end(argptr);

		common->Printf("%s: %s\n",func, text);
	}

	void ConsoleWrite(const char* fmt, ...)
	{
		va_list		argptr;
		char		text[MAX_STRING_CHARS];

		va_start(argptr, fmt);
		idStr::vsnPrintf(text, sizeof(text), fmt, argptr);
		va_end(argptr);

		common->Printf("%s\n", text);
	}

	void Warning(const char* fmt, ...)
	{
		va_list		argptr;
		char		text[MAX_STRING_CHARS];
		idThread *	thread;

		va_start(argptr, fmt);
		idStr::vsnPrintf(text, sizeof(text), fmt, argptr);
		va_end(argptr);

		thread = idThread::CurrentThread();
		if (thread)
		{
			thread->Warning("PreyRun: %s\n", text);
		}
		else
		{
			common->Warning("PreyRun: %s\n", text);
		}
	}

	void Error(const char* fmt, ...)
	{
		va_list		argptr;
		char		text[MAX_STRING_CHARS];
		idThread *	thread;

		va_start(argptr, fmt);
		idStr::vsnPrintf(text, sizeof(text), fmt, argptr);
		va_end(argptr);

		thread = idThread::CurrentThread();
		if (thread)
		{
			thread->Error("PreyRun: %s\n", text);
		}
		else
		{
			common->Error("PreyRun: %s\n", text);
		}
	}
}
