#include "../idLib/precompiled.h"
#pragma hdrstop

#include "Logging.hpp"
#include "../game/Game_local.h"

namespace pr
{
	void WriteToLogFile(const char* pre, const char* string);

	void Log(const char* fmt, ...)
	{
		va_list		argptr;
		char		text[MAX_STRING_CHARS];

		va_start(argptr, fmt);
		idStr::vsnPrintf(text, sizeof(text), fmt, argptr);
		va_end(argptr);

		WriteToLogFile("", text);

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

		WriteToLogFile("[Debug]", text);

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

		WriteToLogFile(func, text);

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

		WriteToLogFile("[WARNING]", text);

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

		WriteToLogFile("[ERROR]", text);

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

	void WriteToLogFile(const char* pre, const char* string)
	{
		if (pr::Cvar::log.GetBool())
		{
			idStr str;

			if (pre != "")
			{
				sprintf(str, "%s: %s\n", pre, string);
			}
			else
			{
				sprintf(str, "%s\n", string);
			}

			auto logFile = fileSystem->OpenFileWrite(pr::LogFilePath, "fs_savepath");
			logFile->Write(str.c_str(),strlen(str.c_str()));
			fileSystem->CloseFile(logFile);
		}
	}
}
