#include "../idLib/precompiled.h"
#pragma hdrstop

#include "PreyRun.hpp"
#include "Backup.hpp"

namespace pr
{
	constexpr char pr_backuptmr_path[] { "backuptmr" };

	PR_FINLINE double GetBackupTime();
	PR_FINLINE double GetBackupRTATime();

	static void WriteToFile(const char* mapName)
	{
		// Open the file
		auto file = fileSystem->OpenFileWrite(pr_backuptmr_path, "fs_savepath");

		if (file == nullptr)
		{
#ifdef PR_DEBUG
			pr::FunctionLog(__FUNCTION__, "Couldn't open %s", pr_backuptmr_path);
#endif // PR_DEBUG
			return;
		}

		file->WriteBool(true); // Is this a valid backuptmr file? : Yes so we actually can recover the time from this file!
		file->WriteFloat(GetBackupTime()); // The actual time returned by pr_gametimer.ClockTicks()
		file->WriteFloat(GetBackupRTATime()); // The actual time returned by pr_rtatimer.ClockTicks()
		file->WriteString(mapName); // The map you were on when the timer was last saved

		// Close the opened file
		fileSystem->CloseFile(file);
	}

	void WriteBackupTime(const char* mapName)
	{
		if (pr::Cvar::timer_backup_interval.GetFloat() > 0.0f)
		{
			static auto last_time = std::chrono::steady_clock::now() - std::chrono::milliseconds(static_cast<long long>(pr::Cvar::timer_backup_interval.GetFloat()) + 1);

			auto now = std::chrono::steady_clock::now();

			if (now >= last_time + std::chrono::milliseconds(static_cast<long long>(pr::Cvar::timer_backup_interval.GetFloat())))
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
			pr::FunctionLog(__FUNCTION__, "Couldn't open %s", pr_backuptmr_path);
#endif // PR_DEBUG
			return;
		}

		bool isValid { false };
		float ingameTimer;
		float rtaTimer;
		idStr mapName;

		file->ReadBool(isValid);
		if (!isValid)
		{
#ifdef PR_DBG_BACKUP
			pr::FunctionLog(__FUNCTION__, "backuptimer is invalid");
#endif // PR_DBG_BACKUP
			return;
		}

		file->ReadFloat(ingameTimer);
		file->ReadFloat(rtaTimer);
		file->ReadString(mapName);

#ifdef PR_DBG_BACKUP
		pr::FunctionLog(__FUNCTION__, "ingameTimer = %f, rtaTimer = %f, mapName = %s", ingameTimer, rtaTimer, mapName.c_str());
#endif // PR_DBG_BACKUP

		// Are we on the correct map to recover our time?
		if (static_cast<idStr>(cMap) == mapName)
		{
			// Set the timers
			pr::Timer::inGame.SetCT(ingameTimer);
			pr::Timer::RTA.SetCT(rtaTimer);
			// Let the timer continue
			pr::Timer::running = true;

#ifdef PR_DEBUG
			pr::FunctionLog(__FUNCTION__, "Successfully recoverd backup time: %f", ingameTimer);
			pr::FunctionLog(__FUNCTION__, "Successfully recoverd backup RTA time: %f", rtaTimer);
#endif // PR_DEBUG
		}
#ifdef PR_DBG_BACKUP
		else
		{
			pr::FunctionLog(__FUNCTION__, "Wrong map to recover time. CurrentMap=%s mapName=%s", cMap, mapName.c_str());
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
			pr::FunctionLog(__FUNCTION__, "Couldn't open file: %s", pr_backuptmr_path);
#endif // PR_DBG_BACKUP
			return;
		}

		// Mark the file as invalid
		file->WriteBool(false);

		fileSystem->CloseFile(file);

#ifdef PR_DBG_BACKUP
		pr::FunctionLog(__FUNCTION__, "Successfully, cleared backup timer");
#endif // PR_DBG_BACKUP
	}

	PR_FINLINE double GetBackupTime()
	{
#ifdef PR_DBG_BACKUP_GETTIME
		pr::FunctionLog(__FUNCTION__, "%f", pr_gametimer.ClockTicks());
#endif // PR_DBG_BACKUP_GETTIME
		return pr::Timer::inGame.ClockTicks();
	}

	PR_FINLINE double GetBackupRTATime()
	{
#ifdef PR_DBG_BACKUP_GETTIME
		pr::FunctionLog(__FUNCTION__, "%f", pr_rtatimer.ClockTicks());
#endif // PR_DBG_BACKUP_GETTIME
		return pr::Timer::RTA.ClockTicks();
	}
}
