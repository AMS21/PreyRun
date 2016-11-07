#pragma once

#include "../game/Game_local.h"

#include "StdLib.hpp"
#include "GameTimer.hpp"

namespace pr
{
	void WriteBackupTime(const char * mapName);
	void LoadBackupTimer(const char* cMap);
	void ClearBackupTimer();

	ID_INLINE double GetBackupTime();
}