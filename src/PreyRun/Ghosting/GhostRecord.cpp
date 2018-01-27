#include "../../idLib/precompiled.h"
#pragma hdrstop

#include "GhostRecord.hpp"

#include "../Constants.hpp"
#include "../Cvar.hpp"
#include "../Logging.hpp"
#include "../PreyRun.hpp"
#include "../Utility.hpp"

#include "../../game/Game_local.h"

#include <ctime>

namespace pr
{
	bool gh_isRecording = false;
	uint16_t gh_recordFrameCount = 0;
	uint64_t gh_recordTime = 0;

	idFile* gh_demoFile;
	idStr currentMap;
	idVec3 lastPos;

	idStr GetAutoRecordFileName()
	{
		idStr fileName { "Ghosts/" };

		const auto now { std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };

		char buf[100] = { 0 };
		std::strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", std::localtime(&now));
		fileName.Append(buf);

		fileName.Append(pr::GhostingDemoPostfix);

		return fileName;
	}

	void StartRecordingGhost(const char * filePath)
	{
		if (gh_isRecording)
		{
			ConsoleWrite("Already recording!");
			return;
		}

		Log("Recording ghosting demo to: %s", filePath);

		gh_demoFile = fileSystem->OpenFileWrite(filePath, "fs_savepath");

		if (gh_demoFile == nullptr)
		{
			pr::DebugWarning("Couldn't open %s", filePath);
			return;
		}

		gh_isRecording = true;
		gh_recordFrameCount = 0;
		gh_recordTime = 0;
		currentMap = gameLocal.GetMapName();
		lastPos = vec3_origin;

		// Write Header
		gh_demoFile->WriteUnsignedInt(pr::GhostingDemoVersion);
		gh_demoFile->WriteString(pr::Cvar::gh_name.GetString());
		gh_demoFile->WriteString(currentMap.c_str());
		gh_demoFile->WriteVec3(idVec3(pr::Cvar::gh_color_r.GetInteger(), pr::Cvar::gh_color_g.GetInteger(), pr::Cvar::gh_color_b.GetInteger()));
	}

	void StopRecordingGhost()
	{
		Log("Stopped recording to: %s", gh_demoFile->GetName());

		const auto time = ms2time(gh_recordTime);

		Log("Recorded %u frames. Time %02d:%02d:%02d.%03d", gh_recordFrameCount, time.hours, time.minutes, time.seconds, time.milliseconds);

		gh_demoFile->WriteUnsignedChar(GhostDemoInfo::DemoEnd);

		fileSystem->CloseFile(gh_demoFile);
		gh_isRecording = false;
	}

	void GhostingDemoTick(const int FTdelta)
	{
		auto const player = gameLocal.GetLocalPlayer();

		if (gameLocal.GetMapName() != currentMap)
		{
			currentMap = gameLocal.GetMapName();

			gh_demoFile->WriteUnsignedChar(GhostDemoInfo::MapChange);
			gh_demoFile->WriteString(currentMap.c_str());
		}

		gh_demoFile->WriteUnsignedChar(GhostDemoInfo::Position);

		// Write frame delta
		gh_demoFile->WriteUnsignedInt(FTdelta);

		// Calculate the middle of the player model
		const auto dist = (gameLocal.GetLocalPlayer()->GetEyePosition() - gameLocal.GetLocalPlayer()->GetOrigin()) / 2;
		const auto pos = gameLocal.GetLocalPlayer()->GetOrigin() + dist;

		// Write current position delta
		gh_demoFile->WriteVec3(pos - lastPos);
		lastPos = pos;

		gh_recordFrameCount++;
		gh_recordTime += FTdelta;
	}
} // End of namespace: pr
