#include "../idLib/precompiled.h"
#pragma hdrstop

// Credits to Ivan Molodetskikh (Yalter) and Chong Jiang Wei (Matherunner) for their interprocess.cpp from BunnymodXT (https://github.com/YaLTeR/BunnymodXT/blob/master/BunnymodXT/Windows/interprocess.cpp)

#include "interprocess.hpp"

namespace pr
{

	static HANDLE pipe_preysplit = INVALID_HANDLE_VALUE;
	static OVERLAPPED overlapped;
	static bool writing_to_pipe;

	void InitPreySplitPipe()
	{
		if (pr_preysplit.GetBool())
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
			if (pipe_preysplit == INVALID_HANDLE_VALUE) {
#ifdef PR_DEBUG
				gameLocal.Printf("Error opening the PreySplit pipe: %d\n", GetLastError());
#endif // PR_DEBUG
				gameLocal.Printf("PreyRun: PreySplit integration is not available.\n");
				return;
			}
			gameLocal.Printf("PreyRun: Opened the PreySplit pipe.\n");
			pr_preysplit_pipeopen = true;

			std::memset(&overlapped, 0, sizeof(overlapped));
			overlapped.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
			if (overlapped.hEvent == NULL) {
#ifdef PR_DEBUG
				gameLocal.Printf("Error creating an event for overlapped: %d. Closing the PreySplit pipe.\n", GetLastError());
#endif // PR_DEBUG
				gameLocal.Printf("PreyRun: PreySplit integration is not available.\n");
				CloseHandle(pipe_preysplit);
				pipe_preysplit = INVALID_HANDLE_VALUE;
				pr_preysplit_pipeopen = false;
			}
		}
#ifdef PR_DEBUG
		else
		{
			gameLocal.Printf("Skipping PreySplit initialization\n");
		}
#endif // PR_DEBUG
	}

	void ShutdownPreySplitPipe()
	{
		if (pipe_preysplit != INVALID_HANDLE_VALUE) {
			CloseHandle(pipe_preysplit);
			gameLocal.Printf("PreyRun: Closed the PreySplit pipe.\n");
		}
		pipe_preysplit = INVALID_HANDLE_VALUE;
		pr_preysplit_pipeopen = false;

		CloseHandle(overlapped.hEvent);
		std::memset(&overlapped, 0, sizeof(overlapped));
	}

	static void WritePreySplit(const std::vector<char>& data)
	{
		if (pipe_preysplit == INVALID_HANDLE_VALUE)
			return;

		if (writing_to_pipe) {
			if (WaitForSingleObject(overlapped.hEvent, INFINITE) != WAIT_OBJECT_0) {
				// Some weird error?
#ifdef PR_DEBUG
				gameLocal.Printf("WaitForSingleObject failed with %d.\n", GetLastError());
#endif // PR_DEBUG
				DisconnectNamedPipe(pipe_preysplit);
				return WritePreySplit(data);
			}
			writing_to_pipe = false;
		}

		if (!ConnectNamedPipe(pipe_preysplit, &overlapped)) {
			auto err = GetLastError();
			if (err == ERROR_NO_DATA) {
				// Client has disconnected.
				DisconnectNamedPipe(pipe_preysplit);
				return WritePreySplit(data);
			}
			else if (err == ERROR_IO_PENDING) {
				// Waiting for someone to connect.
				return;
			}
			else if (err != ERROR_PIPE_CONNECTED) {
				// Some weird error with pipe?
				// Try remaking it.
#ifdef PR_DEBUG
				gameLocal.Printf("ConnectNamedPipe failed with %d.\n", err);
#endif // PR_DEBUG
				ShutdownPreySplitPipe();
				InitPreySplitPipe();
				return WritePreySplit(data);
			}
		}

		if (!WriteFile(pipe_preysplit, data.data(), data.size(), NULL, &overlapped)) {
			auto err = GetLastError();
			if (err == ERROR_IO_PENDING) {
				// Started writing.
				writing_to_pipe = true;
				return;
			}
			else {
#ifdef PR_DEBUG
				gameLocal.Printf("WriteFile failed with %d.\n", err);
#endif // PR_DEBUG
				DisconnectNamedPipe(pipe_preysplit);
				return;
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

#ifdef PR_DEBUG
		gameLocal.Printf("PreyRunDBG: WriteGameEnd: %02d:%02d:%02d.%03d\n", time.hours, time.minutes, time.seconds, time.milliseconds);
#endif // PR_DEBUG


		WritePreySplit(buf);
	}

	void WriteTime(const Time& time)
	{
		std::vector<char> buf(10);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::TIME);
		AddTimeToBuffer(buf.data() + 2, time);

		if (pr_preysplit_update.GetFloat() > 0.0f)
		{
			static auto last_time = std::chrono::steady_clock::now() - std::chrono::milliseconds(static_cast<long long>(1000 / pr_preysplit_update.GetFloat()) + 1);

			auto now = std::chrono::steady_clock::now();

			if (now >= last_time + std::chrono::milliseconds(static_cast<long long>(1000 / pr_preysplit_update.GetFloat())))
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
		// normal map path   : maps/game/roadhouse.map
		// gets turneded into: roadhouse.map
		map.Replace("maps/game/", "");
#ifdef PR_DEBUG
		gameLocal.Printf("PreyRunDBG: WriteMapChange: Map exited: %s\nPreyRunDBG: WriteMapChange: Time %02d:%02d:%02d.%03d\n", map.c_str(), time.hours, time.minutes, time.seconds, time.milliseconds);
#endif // PR_DEBUG

		int32_t size = static_cast<int32_t>(map.Size());

		std::vector<char> buf(15 + size);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::EVENT);
		buf[2] = static_cast<char>(EventType::MAPCHANGE);
		auto time_size = AddTimeToBuffer(buf.data() + 3, time);

		std::memcpy(buf.data() + 3 + time_size, &size, sizeof(size));
		std::memcpy(buf.data() + 3 + time_size + 4, map.c_str(), size);

		WritePreySplit(buf);
	}

	void WriteTimerReset(const Time& time)
	{
		std::vector<char> buf(11);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::EVENT);
		buf[2] = static_cast<char>(EventType::TIMER_RESET);
		AddTimeToBuffer(buf.data() + 3, time);
#ifdef PR_DEBUG
		gameLocal.Printf("PreyRunDBG: WriteTimerReset: %02d:%02d:%02d.%03d\n", time.hours, time.minutes, time.seconds, time.milliseconds);
#endif // PR_DEBUG

		WritePreySplit(buf);
	}

	void WriteTimerStart(const Time& time)
	{
		std::vector<char> buf(11);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::EVENT);
		buf[2] = static_cast<char>(EventType::TIMER_START);
		AddTimeToBuffer(buf.data() + 3, time);

#ifdef PR_DEBUG
		gameLocal.Printf("PreyRunDBG: WriteTimerStart: %02d:%02d:%02d.%03d\n", time.hours, time.minutes, time.seconds, time.milliseconds);
#endif // PR_DEBUG

		WritePreySplit(buf);
	}

	Time GetTime()
	{
		PR_time_t times;

		if (pr_gametimer.IsRunning())
		{
			pr_gametimer.Stop();
			times = PR_ms2time(pr_gametimer.Milliseconds());
			pr_gametimer.Start();
		}
		else
		{
			times = PR_ms2time(pr_gametimer.Milliseconds());
		}

		return Time{ times.hours, times.minutes, times.seconds, times.milliseconds };
	}
}