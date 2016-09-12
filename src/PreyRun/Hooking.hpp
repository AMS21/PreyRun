#pragma once

#include "../game/Game_local.h"

namespace pr
{
	// Const stuff to avoid magic numbers
	const DWORD DisplayTimeDemo = 0x004576dd;
	const char DisplayTimeDemoOn[] = "\x90\x90\x90\x90\x90\x90";
	const char DisplayTimeDemoOff[] = "\x0F\x84\x34\x01\x00\x00";

	const DWORD DisplayTimeDemoString = 0x007B59E4;
	const char DisplayTimeDemoStringOn[] = "%i frames rendered in %3.3f seconds = %3.3f fps\n";
	const char DisplayTimeDemoStringOff[] = "%i frames rendered in %3.1f seconds = %3.1f fps\n";

	// Public functions

	void timeDemoInit();
	void timeDemoShutdown();

	// Static functions
	
	void static WriteToMemory(DWORD addressToWrite, const char* valueToWrite, int byteNum);
}