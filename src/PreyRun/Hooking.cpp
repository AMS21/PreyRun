#include "../idLib/precompiled.h"
#pragma hdrstop

#include "PreyRun.hpp"
#include "Hooking.hpp"
#include "../framework/licensee.h"
#include <detours.h>

namespace pr
{
	constexpr DWORD StopRecordingRenderDemo = 0x454010;
	void(__cdecl* StopRecordingRenderDemoOriginal)(void* this_ptr);

	constexpr DWORD StartRecordingRenderDemo = 0x4554F0;
	void(__cdecl* StartRecordingRenderDemoOriginal)(void* this_ptr, const char* demoName);

	constexpr DWORD StopPlayingRenderDemo = 0x457610;
	void(__cdecl* StopPlayingRenderDemoOriginal)(void* this_ptr);

	constexpr DWORD StartPlayingRenderDemo = 0x457830;
	int(__cdecl* StartPlayingRenderDemoOriginal)(void* this_ptr, const char* demoName, char a1, BYTE* a2, int* a3, int* a4, int* a5, int* a6, int* a7, int a8);

	constexpr DWORD OneClickLoad = 0x45C47A;
	constexpr char OneClickLoadOn[] { '\x90','\x90' };
	constexpr char OneClickLoadOff[] { '\x74','\x12' };

	constexpr DWORD PreyVersionString = 0x7AC500;
	constexpr char PreyVersionStringNew[16] = ENGINE_VERSION;

	constexpr DWORD PreyVerionFormatString = 0x7AF038;
	constexpr char PreyVersionFormatStringNew[6] = "   %s"; // orig: '%s.%i'

	constexpr DWORD PreyVersionDateString = 0x7AC514;
	constexpr char PreyVersionDateStringNew[12] = __DATE__;

	constexpr DWORD PreyVersionTimeString = 0x7AC520;
	constexpr char PreyVersionTimeStringNew[9] = __TIME__;

	constexpr DWORD MasterServerError = 0x470C8C;
	constexpr char MasterServerErrorOrig[] { '\x75' }; // JNE
	constexpr char MasterServerErrorNew[] { '\xEB' }; // JMP
	constexpr DWORD IdPortError = 0x530C7C;
	constexpr char IdPortErrorOrig[] { '\x75' }; // JNE
	constexpr char IdPortErrorNew[] { '\xEB' }; // JMP

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

	bool static HookJMP(void* pAddress, void* dwJumpTo, unsigned dwLen)
	{
		if (dwLen < 5) { return false; }

		DWORD dwOldProtect;

		// give the paged memory read/write permissions

		VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);

		// calculate the distance between our address and our target location
		// and subtract the 5bytes, which is the size of the jmp
		// (0xE9 0xAA 0xBB 0xCC 0xDD) = 5 bytes

		DWORD dwRelAddr = ((DWORD) dwJumpTo - (DWORD) pAddress) - 5;

		// overwrite the byte at pAddress with the jmp opcode (0xE9)

		*(BYTE*) pAddress = 0xE9;

		// overwrite the next 4 bytes (which is the size of a DWORD)
		// with the dwRelAddr

		*((DWORD *) ((DWORD) pAddress + 0x1)) = dwRelAddr;

		// overwrite the remaining bytes with the NOP opcode (0x90)
		// NOP opcode = No OPeration

		for (DWORD x = 0x5; x < dwLen; x++)
		{
			*((DWORD*) ((DWORD) pAddress + x)) = 0x90;
		}

		// restore the paged memory permissions saved in dwOldProtect

		DWORD dwBkup;
		VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);

		return true;
	}

	void __stdcall PrintDemoTime()
	{
		auto time = PR_ms2time(pr::Timer::demo.Milliseconds());

		pr::Log("Time: %02d:%02d:%02d.%02d", time.hours, time.minutes, time.seconds, time.milliseconds);
	}

	void StopRecordingRenderDemoHook(void* this_ptr)
	{
		//pr::DebugLog("StopRecordingRenderDemoHook");

		pr::Timer::demo.Stop();

		PrintDemoTime();

		StopRecordingRenderDemoOriginal(this_ptr);
	}

	void StartRecordingRenderDemoHook(void* this_ptr, const char* demoName)
	{
		//pr::DebugLog("StartRecordingRenderDemoHook");

		pr::Timer::demo.Start();

		StartRecordingRenderDemoOriginal(this_ptr, demoName);
	}

	void StopPlayingRenderDemoHook(void* this_ptr)
	{
		//pr::DebugLog("StopPlayingRenderDemoHook");

		if (pr::Timer::timedemo)
		{
			pr::Timer::demo.Stop();

			pr::Timer::timedemo = false;

			PrintDemoTime();
		}

		StopPlayingRenderDemoOriginal(this_ptr);
	}

	int StartPlayingRenderDemoHook(void* this_ptr, const char* demoName, char a1, BYTE* a2, int* a3, int* a4, int* a5, int* a6, int* a7, int a8)
	{
		//pr::DebugLog("StartPlayingRenderDemoHook");

		if (!pr::Timer::timedemo)
		{
			pr::Timer::demo.Clear();
			pr::Timer::demo.Start();

			pr::Timer::timedemo = true;
		}

		return StartPlayingRenderDemoOriginal(this_ptr, demoName, a1, a2, a3, a4, a5, a6, a7, a8);
	}

	void hookDemoRecording()
	{
		StopRecordingRenderDemoOriginal = (void(__cdecl*)(void* this_ptr))DetourFunction((PBYTE) StopRecordingRenderDemo, (PBYTE) StopRecordingRenderDemoHook);
		StartRecordingRenderDemoOriginal = (void(__cdecl*)(void* this_ptr, const char* demoName))DetourFunction((PBYTE) StartRecordingRenderDemo, (PBYTE) StartRecordingRenderDemoHook);

		StopPlayingRenderDemoOriginal = (void(__cdecl*)(void* this_ptr))DetourFunction((PBYTE) StopPlayingRenderDemo, (PBYTE) StopPlayingRenderDemoHook);
		StartPlayingRenderDemoOriginal = (int(__cdecl*)(void* this_ptr, const char* demoName, char a1, BYTE* a2, int* a3, int* a4, int* a5, int* a6, int* a7, int a8))DetourFunction((PBYTE) StartPlayingRenderDemo, (PBYTE) StartPlayingRenderDemoHook);

	#ifdef PR_DBG_HOOKING
		pr::DebugLog("Successfully hooked demo recording and playback");
	#endif // PR_DBG_HOOKING
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
}
