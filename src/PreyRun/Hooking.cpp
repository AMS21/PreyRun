#include "../idLib/precompiled.h"
#pragma hdrstop

#include "Hooking.hpp"

#include "Cvar.hpp"
#include "Logging.hpp"

#include "../framework/licensee.h"

namespace pr
{
	constexpr DWORD OneClickLoad = 0x45C47A;
	constexpr char OneClickLoadOn[] { '\x90','\x90' };
	constexpr char OneClickLoadOff[] { '\x74','\x12' };

	constexpr DWORD PreyVersionString = 0x7AC500;
	constexpr char PreyVersionStringNew[16] = ENGINE_VERSION;

	constexpr DWORD PreyVerionFormatString = 0x7AF038;
	constexpr char PreyVersionFormatStringNew[6] = "   %s"; // orig: '%s.%i'

	constexpr DWORD PreyVersionDateString = 0x7AC514;
	constexpr char PreyVersionDateStringNew[12] = PR_CMPL_DATE;

	constexpr DWORD PreyVersionTimeString = 0x7AC520;
	constexpr char PreyVersionTimeStringNew[9] = PR_CMPL_TIME;

	constexpr DWORD MasterServerError = 0x470C8C;
	constexpr char MasterServerErrorOrig[] { '\x75' }; // JNE
	constexpr char MasterServerErrorNew[] { '\xEB' }; // JMP
	constexpr DWORD IdPortError = 0x530C7C;
	constexpr char IdPortErrorOrig[] { '\x75' }; // JNE
	constexpr char IdPortErrorNew[] { '\xEB' }; // JMP

	void static WriteToMemory(DWORD addressToWrite, const char* valueToWrite, const int byteNum) noexcept
	{
		// used to change our file access type, stores the old
		// access type and restores it after memory is written
		unsigned long OldProtection;
		// give that address read and write permissions and store the old permissions at oldProtection
		VirtualProtect(reinterpret_cast<LPVOID>(addressToWrite), byteNum, PAGE_EXECUTE_READWRITE, &OldProtection);

		// write the memory into the program and overwrite previous value
		memcpy(reinterpret_cast<LPVOID>(addressToWrite), valueToWrite, byteNum);

		// reset the permissions of the address back to oldProtection after writting memory
		VirtualProtect(reinterpret_cast<LPVOID>(addressToWrite), byteNum, OldProtection, 0);
	}

	void enableOneClickLoad()
	{
		WriteToMemory(OneClickLoad, OneClickLoadOn, sizeof(OneClickLoadOn));
		oneClickLoad = true;

	#ifdef PR_DBG_HOOKING
		pr::DebugLog("OneClickLoad Enabled");
	#endif // PR_DBG_HOOKING
	}

	void disableOneClickLoad()
	{
		WriteToMemory(OneClickLoad, OneClickLoadOff, sizeof(OneClickLoadOff));
		oneClickLoad = false;

	#ifdef PR_DBG_HOOKING
		pr::DebugLog("OneClickLoad Disabled");
	#endif // PR_DBG_HOOKING
	}

	void hookVersionDisplay()
	{
		WriteToMemory(PreyVersionString, PreyVersionStringNew, sizeof(PreyVersionStringNew));
		WriteToMemory(PreyVerionFormatString, PreyVersionFormatStringNew, sizeof(PreyVersionFormatStringNew));
		WriteToMemory(PreyVersionDateString, PreyVersionDateStringNew, sizeof(PreyVersionDateStringNew));
		WriteToMemory(PreyVersionTimeString, PreyVersionTimeStringNew, sizeof(PreyVersionTimeStringNew));

	#ifdef PR_DBG_HOOKING
		pr::DebugLog("Version display hooked");
	#endif // PR_DBG_HOOKING
	}

	void suppressMasterServerWarning()
	{
		WriteToMemory(MasterServerError, MasterServerErrorNew, sizeof(MasterServerErrorNew));
		WriteToMemory(IdPortError, IdPortErrorNew, sizeof(IdPortErrorNew));

	#ifdef PR_DBG_HOOKING
		pr::DebugLog("Master server warning suppressed");
	#endif
	}

	void unsuppressMasterServerWarning()
	{
		WriteToMemory(MasterServerError, MasterServerErrorOrig, sizeof(MasterServerErrorOrig));
		WriteToMemory(IdPortError, IdPortErrorOrig, sizeof(IdPortErrorOrig));

	#ifdef PR_DBG_HOOKING
		pr::DebugLog("Master server warning unsuppressed");
	#endif
	}
} // End of namespace: pr
