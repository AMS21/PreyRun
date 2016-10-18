#include "../idLib/precompiled.h"
#pragma hdrstop

#include "Hooking.hpp"

namespace pr
{
	void static WriteToMemory(DWORD addressToWrite, const char* valueToWrite, const int byteNum)
	{
		//used to change our file access type, stores the old
		//access type and restores it after memory is written
		unsigned long OldProtection;
		//give that address read and write permissions and store the old permissions at oldProtection
		VirtualProtect(reinterpret_cast<LPVOID>(addressToWrite), byteNum, PAGE_EXECUTE_READWRITE, &OldProtection);

		//write the memory into the program and overwrite previous value
		memcpy(reinterpret_cast<LPVOID>(addressToWrite), valueToWrite, byteNum);

		//reset the permissions of the address back to oldProtection after writting memory
		VirtualProtect(reinterpret_cast<LPVOID>(addressToWrite), byteNum, OldProtection, NULL);
	}

	void timeDemoInit()
	{
		WriteToMemory(DisplayTimeDemo, DisplayTimeDemoOn, sizeof(DisplayTimeDemoOn));
		WriteToMemory(DisplayTimeDemoString, DisplayTimeDemoStringOn, sizeof(DisplayTimeDemoStringOn));
#ifdef PR_DEBUG
		gameLocal.Printf("PreyRunDBG: Hooked timeDemo\n");
#endif // PR_DEBUG
	}

	void timeDemoShutdown()
	{
		WriteToMemory(DisplayTimeDemo, DisplayTimeDemoOff, sizeof(DisplayTimeDemoOff));
		WriteToMemory(DisplayTimeDemoString, DisplayTimeDemoStringOff, sizeof(DisplayTimeDemoStringOff));
#ifdef PR_DEBUG
		gameLocal.Printf("PreyRunDBG: Unhooked timeDemo\n");
#endif // PR_DEBUG
	}
}
