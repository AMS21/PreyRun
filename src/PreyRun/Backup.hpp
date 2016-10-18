#pragma once

#include "../game/Game_local.h"

#include "StdLib.hpp"
#include "Constants.hpp"
#include "GameTimer.hpp"

namespace pr
{
	void WriteBackupTime(const char * mapName);
	void LoadBackupTimer(const char* cMap);
	void ClearBackupTimer();

	inline double GetBackupTime();
}