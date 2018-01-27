#pragma once

namespace pr
{
	/*	Prints: Console, Logfile
		Format: 'PreyRun: %s\n'
	*/
	void Log(const char *fmt, ...);

	/*	Prints: Console, Logfile
		Debug : Will only be printed if the 'PR_DEBUG' macro is set
		Format: 'PreyRun DBG: %s\n'
	*/
	void DebugLog(const char *fmt, ...);

	/*	Prints: Console, Logfile
		Format: '__FUNCTION__: %s\n'
	*/
	void FunctionLog(const char *func, const char *fmt, ...);

	/*	Prints: Console, Logfile
		Debug : Will only be printed if the 'PR_DEBUG' macro is set
		Format: '__FUNCTION__: %s\n'
	*/
	void DebugFunctionLog(const char *func, const char *fmt, ...);

	/*	Prints: Console
		Format: '%s\n'
	*/
	void ConsoleWrite(const char *fmt, ...);

	/*	Prints: Console
		Debug : Will only be printed if the 'PR_DEBUG' macro is set
		Format: '%s\n'
	*/
	void DebugConsoleWrite(const char *fmt, ...);

	/*	Prints: Console, Logfile
		Format: 'PreyRun: %s\n'
	*/
	void Warning(const char *fmt, ...);

	/*	Prints: Console, Logfile
		Debug : Will only be printed if the 'PR_DEBUG' macro is set
		Format: 'PreyRun: %s\n'
	*/
	void DebugWarning(const char *fmt, ...);

	/*	Prints: Console, Logfile
		Format: 'PreyRun DBG: %s\n'
	*/
	void Error(const char *fmt, ...);

	/*	Prints: Console, Logfile
		Debug : Will only be printed if the 'PR_DEBUG' macro is set
		Format: 'PreyRun DBG: %s\n'
	*/
	void DebugError(const char *fmt, ...);
} // End of namespace: pr
