#pragma once
//#include "../game/game_local.h"
#include <Windows.h>

// Realy hacking way to use stdlib functions
#undef strcmp
#undef strncmp
#undef StrCmpN
#undef strcmpi
#undef StrCmpI
#undef stricmp
#undef _stricmp
#undef strcasecmp
#undef strnicmp
#undef _strnicmp
#undef _memicmp
#undef StrCmpNI
#undef snprintf
#undef _snprintf
#undef vsnprintf
#undef _vsnprintf

#include <vector>
#include <cstring>

#define PREYSPLIT_PIPE_NAME "PreyRun-PreySplit"

namespace pr
{

	enum class MessageType : unsigned char {
		TIME = 0x00,
		EVENT = 0x01
	};

	enum class EventType : unsigned char {
		GAMEEND = 0x00,
		MAPCHANGE = 0x01,
		TIMER_RESET = 0x02,
		TIMER_START = 0x03,
	};

	struct Time
	{
		uint32_t hours;
		uint8_t minutes;
		uint8_t seconds;
		uint16_t milliseconds;
	};

	void InitPreySplitPipe();
	void ShutdownPreySplitPipe();

	void WriteGameEnd(const Time& time);
	void WriteMapChange(const Time& time, const std::string& map);
	void WriteTimerReset(const Time& time);
	void WriteTimerStart(const Time& time);
}

#define strcmp			idStr::Cmp		// use_idStr_Cmp
#define strncmp			use_idStr_Cmpn
#define StrCmpN			use_idStr_Cmpn
#define strcmpi			use_idStr_Icmp
#define StrCmpI			use_idStr_Icmp
#define stricmp			idStr::Icmp		// use_idStr_Icmp
#define _stricmp		use_idStr_Icmp
#define strcasecmp		use_idStr_Icmp
#define strnicmp		use_idStr_Icmpn
#define _strnicmp		use_idStr_Icmpn
#define _memicmp		use_idStr_Icmpn
#define StrCmpNI		use_idStr_Icmpn
#define snprintf		use_idStr_snPrintf
#define _snprintf		use_idStr_snPrintf
#define vsnprintf		use_idStr_vsnPrintf
#define _vsnprintf		use_idStr_vsnPrintf