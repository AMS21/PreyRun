#pragma once

#include "../game/Game_local.h"

namespace pr
{
	void timeDemoInit();
	void timeDemoShutdown();

	void enableOneClickLoad();
	void disableOneClickLoad();

	void hookVersionDisplay();
}
