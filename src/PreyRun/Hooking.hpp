#pragma once

#include "../game/Game_local.h"

namespace pr
{
	void enableOneClickLoad();
	void disableOneClickLoad();

	void hookDemoRecording();

	void hookVersionDisplay();

	void suppressMasterServerWarning();
	void unsuppressMasterServerWarning();
}
