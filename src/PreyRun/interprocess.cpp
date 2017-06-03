#include "../idLib/precompiled.h"
#pragma hdrstop

// Credits to Ivan Molodetskikh (Yalter) and Chong Jiang Wei (Matherunner) for their interprocess.cpp from BunnymodXT (https://github.com/YaLTeR/BunnymodXT/blob/master/BunnymodXT/Windows/interprocess.cpp)

#include "PreyRun.hpp"
#include "interprocess.hpp"
#include "Thread.hpp"

namespace pr
{
	static HANDLE pipe_preysplit = INVALID_HANDLE_VALUE;
	static OVERLAPPED overlapped;
	std::atomic<bool> writing_to_pipe;
	//static bool writing_to_pipe;

	void InitPreySplitPipe()
	{
		if (pr::Cvar::preysplit.GetBool())
		{
			pipe_preysplit = CreateNamedPipe(
				"\\\\.\\pipe\\" PREYSPLIT_PIPE_NAME,
				PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED,
				PIPE_TYPE_MESSAGE | PIPE_REJECT_REMOTE_CLIENTS,
				1,
				256 * 1000,
				0,
				0,
				NULL);
			if (pipe_preysplit == INVALID_HANDLE_VALUE)
			{
				pr::DebugLog("Error opening the PreySplit pipe: %d", GetLastError());
				gameLocal.Warning("PreyRun: PreySplit integration is not available.\n");
				return;
			}

			pr::Log("Opened the PreySplit pipe.");
			pr::preysplit_pipeopen = true;

			std::memset(&overlapped, 0, sizeof(overlapped));
			overlapped.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

			if (overlapped.hEvent == NULL)
			{
				pr::DebugLog("Error creating an event for overlapped: %d. Closing the PreySplit pipe.", GetLastError());

				pr::Warning("PreySplit integration is not available!");
				CloseHandle(pipe_preysplit);
				pipe_preysplit = INVALID_HANDLE_VALUE;
				pr::preysplit_pipeopen = false;
			}

			prThread::launch(PreySplitMain);
		}
#ifdef PR_DEBUG
		else
		{
			pr::Log("Skipping PreySplit initialization");
		}
#endif // PR_DEBUG
	}

	void ShutdownPreySplitPipe()
	{
		if (pipe_preysplit != INVALID_HANDLE_VALUE)
		{
			CloseHandle(pipe_preysplit);
			pr::Log("Closed the PreySplit pipe.");
		}

		pipe_preysplit = INVALID_HANDLE_VALUE;
		pr::preysplit_pipeopen = false;

		CloseHandle(overlapped.hEvent);
		std::memset(&overlapped, 0, sizeof(overlapped));
	}

	void PreySplitMain()
	{
		while (pr::preysplit_pipeopen)
		{
			if (pr::Timer::running)
			{
				WriteTime(pr::GetTime());
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(16)); // about every frame

			//std::this_thread::sleep_for(std::chrono::milliseconds(pr::Cvar::preysplit_update.GetInteger())); // pr::Cvar::preysplit_update is not thread safe, but woud be better
		}
	}

	static void WritePreySplit(const std::vector<char>& data)
	{
		if (pipe_preysplit == INVALID_HANDLE_VALUE) { return; }

		if (writing_to_pipe)
		{
			if (WaitForSingleObject(overlapped.hEvent, INFINITE) != WAIT_OBJECT_0)
			{
				// Some weird error?
				pr::DebugLog("WaitForSingleObject failed with %d.", GetLastError());

				DisconnectNamedPipe(pipe_preysplit);
				return WritePreySplit(data);
			}

			writing_to_pipe = false;
		}

		if (!ConnectNamedPipe(pipe_preysplit, &overlapped))
		{
			auto err = GetLastError();
			if (err == ERROR_NO_DATA)
			{
				// Client has disconnected.
				DisconnectNamedPipe(pipe_preysplit);
				return WritePreySplit(data);
			}

			if (err == ERROR_IO_PENDING)
			{
				// Waiting for someone to connect.
				return;
			}

			if (err != ERROR_PIPE_CONNECTED)
			{
				// Some weird error with pipe?
				// Try remaking it.
				pr::DebugLog("ConnectNamedPipe failed with %d.", err);

				ShutdownPreySplitPipe();
				InitPreySplitPipe();
				return WritePreySplit(data);
			}
		}

		if (!WriteFile(pipe_preysplit, data.data(), data.size(), NULL, &overlapped))
		{
			auto err = GetLastError();
			if (err == ERROR_IO_PENDING)
			{
				// Started writing.
				writing_to_pipe = true;
			}
			else
			{
				pr::DebugLog("WriteFile failed with %d.", err);
				DisconnectNamedPipe(pipe_preysplit);
			}
		}
	}

	static size_t AddTimeToBuffer(char* buf, const Time& time)
	{
		std::memcpy(buf, &time.hours, sizeof(time.hours));
		std::memcpy(buf + sizeof(time.hours), &time.minutes, sizeof(time.minutes));
		std::memcpy(buf + sizeof(time.hours) + sizeof(time.minutes), &time.seconds, sizeof(time.seconds));
		std::memcpy(buf + sizeof(time.hours) + sizeof(time.minutes) + sizeof(time.seconds), &time.milliseconds, sizeof(time.milliseconds));

		return sizeof(time.hours) + sizeof(time.minutes) + sizeof(time.seconds) + sizeof(time.milliseconds);
	}

	void WriteGameEnd(const Time& time)
	{
		std::vector<char> buf(11);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::EVENT);
		buf[2] = static_cast<char>(EventType::GAMEEND);
		AddTimeToBuffer(buf.data() + 3, time);

#ifdef PR_DBG_INTERPROCESS
		pr::DebugLog("WriteGameEnd: %02d:%02d:%02d.%03d", time.hours, time.minutes, time.seconds, time.milliseconds);
#endif // PR_DBG_INTERPROCESS

		WritePreySplit(buf);
	}

	void WriteTime(const Time& time)
	{
		std::vector<char> buf(10);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::TIME);
		AddTimeToBuffer(buf.data() + 2, time);

		if (pr::Cvar::preysplit_update.GetFloat() > 0.0f)
		{
			static auto last_time = std::chrono::steady_clock::now() - std::chrono::milliseconds(static_cast<long long>(pr::Cvar::preysplit_update.GetFloat()) + 1);

			auto now = std::chrono::steady_clock::now();

			if (now >= last_time + std::chrono::milliseconds(static_cast<long long>(pr::Cvar::preysplit_update.GetFloat())))
			{
				WritePreySplit(buf);
				last_time = now;
			}
		}
		else
		{
			WritePreySplit(buf);
		}
	}

	void WriteMapChange(const Time& time, idStr& map)
	{
		// normal map path    : maps/game/roadhouse.map
		// gets turneded into : roadhouse.map
		auto Map = map.Mid(10, map.Length() - 10);

#ifdef PR_DBG_INTERPROCESS
		pr::DebugLog("WriteMapChange: Map exited: %s", Map.c_str());
		pr::DebugLog("WriteMapChange: Time %02d:%02d:%02d.%03d", time.hours, time.minutes, time.seconds, time.milliseconds);
#endif // PR_DBG_INTERPROCESS

		auto size = static_cast<int32_t>(Map.Size());

		std::vector<char> buf(15 + size);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::EVENT);
		buf[2] = static_cast<char>(EventType::MAPCHANGE);
		auto time_size = AddTimeToBuffer(buf.data() + 3, time);

		std::memcpy(buf.data() + 3 + time_size, &size, sizeof(size));
		std::memcpy(buf.data() + 3 + time_size + 4, Map.c_str(), size);

		WritePreySplit(buf);
	}

	void WriteTimerReset(const Time& time)
	{
		std::vector<char> buf(11);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::EVENT);
		buf[2] = static_cast<char>(EventType::TIMER_RESET);
		AddTimeToBuffer(buf.data() + 3, time);

#ifdef PR_DBG_INTERPROCESS
		pr::DebugLog("WriteTimerReset: %02d:%02d:%02d.%03d", time.hours, time.minutes, time.seconds, time.milliseconds);
#endif // PR_DBG_INTERPROCESS

		WritePreySplit(buf);
	}

	void WriteTimerStart(const Time& time)
	{
		std::vector<char> buf(11);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::EVENT);
		buf[2] = static_cast<char>(EventType::TIMER_START);
		AddTimeToBuffer(buf.data() + 3, time);

#ifdef PR_DBG_INTERPROCESS
		pr::DebugLog("WriteTimerStart: %02d:%02d:%02d.%03d", time.hours, time.minutes, time.seconds, time.milliseconds);
#endif // PR_DBG_INTERPROCESS

		WritePreySplit(buf);
	}

	void WriteBossKill(const Time& time, const idStr& boss)
	{
		auto size = static_cast<int32_t>(boss.Size());

		std::vector<char> buf(15 + size);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::EVENT);
		buf[2] = static_cast<char>(EventType::BOSS_KILL);
		auto time_size = AddTimeToBuffer(buf.data() + 3, time);

		std::memcpy(buf.data() + 3 + time_size, &size, sizeof(size));
		std::memcpy(buf.data() + 3 + time_size + 4, boss.c_str(), size);

#ifdef PR_DBG_INTERPROCESS
		pr::DebugLog("WriteBossKill: Boss: %s Time: %02d:%02d:%02d.%03d", boss.c_str(), time.hours, time.minutes, time.seconds, time.milliseconds);
#endif // PR_DBG_INTERPROCESS

		WritePreySplit(buf);
	}

	void WriteCustomSplit(const Time& time)
	{
		std::vector<char> buf(11);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::EVENT);
		buf[2] = static_cast<char>(EventType::CUSTOM_SPLIT);
		AddTimeToBuffer(buf.data() + 3, time);

#ifdef PR_DBG_INTERPROCESS
		pr::DebugLog("CustomSplit: %02d:%02d:%02d.%03d", time.hours, time.minutes, time.seconds, time.milliseconds);
#endif // PR_DBG_INTERPROCESS

		WritePreySplit(buf);
	}

	Time GetTime()
	{
		auto times = PR_ms2time(pr::Timer::inGame.Milliseconds());

		return Time { times.hours, times.minutes, times.seconds, times.milliseconds };
	}
}
