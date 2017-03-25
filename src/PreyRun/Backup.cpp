#include "../idLib/precompiled.h"
#pragma hdrstop

#include "PreyRun.hpp"
#include "Backup.hpp"

namespace pr
{
	constexpr char pr_backuptmr_path[] { "backuptmr" };

	static void WriteToFile(const char* mapName)
	{
		// Open the file
		auto file = fileSystem->OpenFileWrite(pr_backuptmr_path, "fs_savepath");

		if (file == nullptr)
		{
#ifdef PR_DEBUG
			gameLocal.Printf("pr::WriteToFile Couldn't open %s\n", pr_backuptmr_path);
#endif // PR_DEBUG
			return;
		}

		file->WriteBool(true); // Is this a valid backuptmr file? : Yes so we actually can recover the time from this file!
		file->WriteFloat(GetBackupTime()); // The actual time returned by pr_gametimer.ClockTicks()
		file->WriteString(mapName); // The map you were on when the timer was last saved

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
		auto file = fileSystem->OpenFileRead(pr_backuptmr_path);

		if (file == nullptr)
		{
#ifdef PR_DEBUG
			gameLocal.Printf("pr::LoadBackupTimer Couldn't open %s\n", pr_backuptmr_path);
#endif // PR_DEBUG
			return;
		}

		bool isValid { false };
		float ms;
		idStr mapName;

		file->ReadBool(isValid);
		if (!isValid)
		{
#ifdef PR_DBG_BACKUP
			gameLocal.Printf("pr::LoadBackupTimer backuptimer is invalid\n");
#endif // PR_DBG_BACKUP
			return;
		}

		file->ReadFloat(ms);
		file->ReadString(mapName);

#ifdef PR_DBG_BACKUP
		gameLocal.Printf("pr::LoadBackupTimer ms=%f mapName=%s\n", ms, mapName.c_str());
#endif // PR_DBG_BACKUP

		// Are we on the correct map to recover our time?
		if (static_cast<idStr>(cMap) == mapName)
		{
			// Set the timer
			pr_gametimer.SetCT(ms);
			// Let the timer continue
			pr_gametimer_running = true;

#ifdef PR_DEBUG
			gameLocal.Printf("pr::LoadBackupTimer Successfully recoverd backup time: %f\n", ms);
#endif // PR_DEBUG
		}
#ifdef PR_DBG_BACKUP
		else
		{
			gameLocal.Printf("pr::LoadBackupTimer wrong map to recover time. CurrentMap=%s mapName=%s\n", cMap, mapName.c_str());
		}
#endif // PR_DBG_BACKUP

		// Close the opened file
		fileSystem->CloseFile(file);
	}

	void ClearBackupTimer()
	{
		// Open the file
		auto file = fileSystem->OpenFileWrite(pr_backuptmr_path, "fs_savepath");

		if (file == nullptr)
		{
#ifdef PR_DBG_BACKUP
			gameLocal.Printf("pr::ClearBackupTimer Coud'nt open file\n");
#endif // PR_DBG_BACKUP
			return;
		}

		// Mark the file as invalid
		file->WriteBool(false);

		fileSystem->CloseFile(file);

#ifdef PR_DBG_BACKUP
		gameLocal.Printf("pr::ClearBackupTimer successfully cleared backup timer\n");
#endif // PR_DBG_BACKUP
	}

	ID_INLINE double GetBackupTime()
	{
#ifdef PR_DBG_BACKUP_GETTIME
		gameLocal.Printf("pr::GetBackupTime = %f\n", pr_gametimer.ClockTicks());
#endif // PR_DBG_BACKUP_GETTIME
		return pr_gametimer.ClockTicks();
	}
}
