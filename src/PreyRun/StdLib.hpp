#pragma once

// Really hacky way to use stdlib functions
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


// Includes
#include <vector>
#include <cstring>
#include <chrono>
#include <array>
#include <algorithm>
#include <utility>
#include <stdint.h>
#include <thread>
#include <mutex>
#include <functional>
#include <utility>
#include <atomic>
#include <future>
// No-Includes


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
