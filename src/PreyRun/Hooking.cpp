#include "../idLib/precompiled.h"
#pragma hdrstop

#include "PreyRun.hpp"
#include "Hooking.hpp"
#include "../framework/licensee.h"

namespace pr
{
	// Const stuff to avoid magic numbers
	constexpr DWORD DisplayTimeDemo = 0x4576dd;
	constexpr char DisplayTimeDemoOn[] = "\x90\x90\x90\x90\x90\x90";
	constexpr char DisplayTimeDemoOff[] = "\x0F\x84\x34\x01\x00\x00";

	constexpr DWORD DisplayTimeDemoString = 0x7B59E4;
	constexpr char DisplayTimeDemoStringOn[] = "%i frames rendered in %3.3f seconds = %3.3f fps\n";
	constexpr char DisplayTimeDemoStringOff[] = "%i frames rendered in %3.1f seconds = %3.1f fps\n";

	constexpr DWORD PreyVersionString = 0x7AC500;
	constexpr char PreyVersionStringNew[] = ENGINE_VERSION;

	constexpr DWORD PreyVerionFormatString = 0x7AF038;
	constexpr char PreyVersionFormatStringNew[] = "   %s"; // orig: '%s.%i'

	constexpr DWORD PreyVersionDateString = 0x7AC514;
	constexpr char PreyVersionDateStringNew[] = __DATE__;

	constexpr DWORD PreyVersionTimeString = 0x7AC520;
	constexpr char PreyVersionTimeStringNew[] = __TIME__;

	void static WriteToMemory(DWORD addressToWrite, const char* valueToWrite, const int byteNum)
	{
		// used to change our file access type, stores the old
		// access type and restores it after memory is written
		unsigned long OldProtection;
		// give that address read and write permissions and store the old permissions at oldProtection
		VirtualProtect(reinterpret_cast<LPVOID>(addressToWrite), byteNum, PAGE_EXECUTE_READWRITE, &OldProtection);

		// write the memory into the program and overwrite previous value
		memcpy(reinterpret_cast<LPVOID>(addressToWrite), valueToWrite, byteNum);

		// reset the permissions of the address back to oldProtection after writting memory
		VirtualProtect(reinterpret_cast<LPVOID>(addressToWrite), byteNum, OldProtection, NULL);
	}

	void timeDemoInit()
	{
		WriteToMemory(DisplayTimeDemo, DisplayTimeDemoOn, sizeof(DisplayTimeDemoOn));
		WriteToMemory(DisplayTimeDemoString, DisplayTimeDemoStringOn, sizeof(DisplayTimeDemoStringOn));
#ifdef PR_DEBUG
		gameLocal.Printf("PreyRun DBG: Hooked timeDemo\n");
#endif // PR_DEBUG
	}

	void timeDemoShutdown()
	{
		WriteToMemory(DisplayTimeDemo, DisplayTimeDemoOff, sizeof(DisplayTimeDemoOff));
		WriteToMemory(DisplayTimeDemoString, DisplayTimeDemoStringOff, sizeof(DisplayTimeDemoStringOff));
#ifdef PR_DEBUG
		gameLocal.Printf("PreyRun DBG: Unhooked timeDemo\n");
#endif // PR_DEBUG
	}

	void hookVersionDisplay()
	{
		WriteToMemory(PreyVersionString, PreyVersionStringNew, sizeof(PreyVersionStringNew));
		WriteToMemory(PreyVerionFormatString, PreyVersionFormatStringNew, sizeof(PreyVersionFormatStringNew));
		WriteToMemory(PreyVersionDateString, PreyVersionDateStringNew, sizeof(PreyVersionDateStringNew));
		WriteToMemory(PreyVersionTimeString, PreyVersionTimeStringNew, sizeof(PreyVersionTimeStringNew));
	}
}
