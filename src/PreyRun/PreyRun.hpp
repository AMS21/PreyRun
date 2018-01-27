#pragma once

#define PR_STR_HELPER(x) #x
#define PR_STR(x) PR_STR_HELPER(x)

#if defined(WIN32) || defined(_WIN32)
#	define PR_FINLINE __forceinline
#endif // WIN32 || _WIN32

#if defined(MACOS_X) || defined(__APPLE__)
#	define PR_FINLINE inline
#endif // MACOS_X || __APPLE__

#ifdef __linux__
#	define PR_FINLINE inline
#endif // __linux__

// PreyRun Version
#define PR_VERSION_MAJOR 1
#define PR_VERSION_MINOR 6
#define PR_VERSION_PATCH 0

#define PR_VERSION PR_STR(PR_VERSION_MAJOR) "." PR_STR(PR_VERSION_MINOR) "." PR_STR(PR_VERSION_PATCH)

// Compile date and time
#define PR_CMPL_DATE __DATE__
#define PR_CMPL_TIME __TIME__
#define PR_CMPL_DATETIME PR_CMPL_DATE " " PR_CMPL_TIME

// The current version of the ghosting demo file format
// Changing this number will make it impossible to load any ghosting demos from previous versions!
#define PR_GHOSTING_DEMO_VERSION 1

namespace pr
{
	constexpr char VersionString[] { PR_VERSION };
	constexpr unsigned VersionMajor { PR_VERSION_MAJOR };
	constexpr unsigned VersionMinor { PR_VERSION_MINOR };
	constexpr unsigned VersionPatch { PR_VERSION_PATCH };

	constexpr unsigned GhostingDemoVersion { PR_GHOSTING_DEMO_VERSION };

	constexpr char LogFilePath[] { "PreyRun.log" };
	constexpr char GhostingDemoPostfix[] { ".gh" };
}

// Debugging PreyRun
#ifdef PR_DEBUG
/* AutoCmd.cpp // Autocmd.hpp */
//#	define PR_DBG_AUTOCMD

/* Backup.cpp // Backup.hpp */
//#	define PR_DBG_BACKUP
//#	define PR_DBG_BACKUP_GETTIME

/* GameTimer.cpp // GameTimer.hpp */
//#	define PR_DBG_GAMETIMER
//#	define PR_DBG_GAMETIMER_GET_TIME
//#	define PR_DBG_GAMETIMER_GET_CLOCKTICKS

/* interprocess.cpp // interprocess.hpp */
#	define PR_DBG_INTERPROCESS

/* Hooking.cpp // Hooking.hpp */
#	define PR_DBG_HOOKING

/* GhostEntity.cpp // GhostEntity.hpp */
#	define PR_DBG_GHOST_ENTITY

/* GhostManager.cpp // GhostManager.hpp */
#	define PR_DBG_GHOST_MANANGER

/* GhostRecord.cpp // GhostRecord.hpp */
#	define PR_DBG_GHOST_RECORD

// Used to mark code which is still being worked on
#	define PR_DEVELOP

#	define PR_BUILD_CONFIGURATION "Pre-Release"

#else
#	define PR_BUILD_CONFIGURATION "Release"
#endif // PR_DEBUG
