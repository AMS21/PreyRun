#include "../idLib/precompiled.h"
#pragma hdrstop

#include "Backup.hpp"

namespace pr
{
	constexpr char* PR_BACKUPTMR_PATH{"backuptmr"};

	static void WriteToFile(const char* mapName)
	{
		// Open the file
		auto file = fileSystem->OpenFileWrite(PR_BACKUPTMR_PATH, "fs_savepath");

		if (file == NULL)
		{
			gameLocal.Printf("Couldn't open %s\n", PR_BACKUPTMR_PATH);
			return;
		}

		file->WriteBool(true);
		file->WriteFloat(GetBackupTime());
		file->WriteString(mapName);

		// Close the opened file
		fileSystem->CloseFile(file);
	}

	void WriteBackupTime(const char* mapName)
	{
		if (pr_timer_backup_interval.GetFloat() > 0.0f)
		{
			static auto last_time = std::chrono::steady_clock::now() - std::chrono::milliseconds(static_cast<long long>(pr_timer_backup_interval.GetFloat()) + 1);

			auto now = std::chrono::steady_clock::now();

			if (now >= last_time + std::chrono::milliseconds(static_cast<long long>(pr_timer_backup_interval.GetFloat())))
			{
				WriteToFile(mapName);
				last_time = now;
			}
		}
		else
		{
			WriteToFile(mapName);
		}
	}

	void LoadBackupTimer(const char* cMap)
	{
		// Open the file
		auto file = fileSystem->OpenFileRead(PR_BACKUPTMR_PATH);
		if (file == NULL)
		{
#ifdef PR_DEBUG
			gameLocal.Printf("Couldn't open %s\n", PR_BACKUPTMR_PATH);
#endif // PR_DEBUG
			return;
		}

		bool isValid{false};
		float ms;
		idStr mapName;

		file->ReadBool(isValid);
		if (!isValid) {	return;}

		file->ReadFloat(ms);
		file->ReadString(mapName);

#ifdef PR_DEBUG
		gameLocal.Printf("Read time: %f\n", ms);
#endif // PR_DEBUG

		// Are we on the correct map to recover our time?
		if (static_cast<idStr>(cMap) == mapName)
		{
			// Set the timer
			pr_gametimer.SetCT(ms);
			// Let the timer continue
			//pr_gametimer.Start();
			pr_gametimer_running = true;

#ifdef PR_DEBUG
			gameLocal.Printf("Successfully recoverd backup time: %f\n", ms);
#endif // PR_DEBUG
		}

		// Close the opened file
		fileSystem->CloseFile(file);
	}

	void ClearBackupTimer()
	{
		// Open the file
		auto file = fileSystem->OpenFileWrite(PR_BACKUPTMR_PATH, "fs_savepath");

		if (file == NULL) { return; }

		// Mark the file as invalid
		file->WriteBool(false);

		fileSystem->CloseFile(file);
	}

	inline double GetBackupTime()
	{
		return pr_gametimer.ClockTicks();
	}
}
