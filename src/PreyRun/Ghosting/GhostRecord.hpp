#pragma once

#include <cstdint>

class idStr;

namespace pr
{
	extern bool gh_isRecording;
	extern uint16_t gh_recordFrameCount;
	extern uint64_t gh_recordTime;

	idStr GetAutoRecordFileName();

	void StartRecordingGhost(const char* filePath);

	void StopRecordingGhost();

	void GhostingDemoTick(const int FTdelta);
} // End of namespace: pr
