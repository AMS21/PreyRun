#pragma once

#include "PreyRun.hpp"
#include "StdLib.hpp"

namespace pr
{
	/*	Prints: Console, Logfile
		Format: 'PreyRun: %s\n'
	*/
	void Log(const char *fmt, ...);

	/*	Prints: Console, Logfile
		Debug : Will only print if the 'PR_DEBUG' macro is set
		Format: 'PreyRun DBG: %s\n'
	*/
	void DebugLog(const char *fmt, ...);

	/*	Prints: Console, Logfile
		Format: '__FUNCTION__: %s\n'
	*/
	void FunctionLog(const char *func, const char *fmt, ...);

	/*	Prints: Console
		Format: '%s\n'
	*/
	void ConsoleWrite(const char *fmt, ...);

	/*	Prints: Console, Logfile
		Format: 'PreyRun: %s\n'
	*/
	void Warning(const char *fmt, ...);

	/*	Prints: Console, Logfile
		Format: 'PreyRun: %s\n'
	*/
	void Error(const char *fmt, ...);
}
