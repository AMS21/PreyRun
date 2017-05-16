#pragma once

#if defined(WIN32) || defined(_WIN32)
#define PR_FINLINE __forceinline
#endif // WIN32 || _WIN32

#if defined(MACOS_X) || defined(__APPLE__)
#define PR_FINLINE inline
#endif // MACOS_X || __APPLE__

#ifdef __linux__
#define PR_FINLINE inline
#endif // __linux__

#ifdef PR_DEBUG
/* AutoCmd.cpp // Autocmd.hpp */
//#define PR_DBG_AUTOCMD

/* Backup.cpp // Backup.hpp */
//#define PR_DBG_BACKUP
//#define PR_DBG_BACKUP_GETTIME

/* GameTimer.cpp // GameTimer.hpp */
//#define PR_DBG_GAMETIMER
//#define PR_DBG_GAMETIMER_GET_TIME
//#define PR_DBG_GAMETIMER_GET_CLOCKTICKS

/* interprocess.cpp // interprocess.hpp */
#define PR_DBG_INTERPROCESS

#endif // PR_DEBUG
