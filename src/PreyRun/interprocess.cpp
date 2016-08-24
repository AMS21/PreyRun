#include "../idLib/precompiled.h"
#pragma hdrstop

#include "interprocess.hpp"

namespace pr
{

	static HANDLE pipe_preysplit = INVALID_HANDLE_VALUE;
	static OVERLAPPED overlapped;
	static bool writing_to_pipe;

	void InitPreySplitPipe()
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
			gameLocal.Printf("Error opening the PreySplit pipe: %d\n", GetLastError());
			gameLocal.Printf("PreySplit integration is not available.\n");
			return;
		}
		gameLocal.Printf("Opened the PreySplit pipe.\n");

		std::memset(&overlapped, 0, sizeof(overlapped));
		overlapped.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
		if (overlapped.hEvent == NULL) {
			gameLocal.Printf("Error creating an event for overlapped: %d. Closing the PreySplit pipe.\n", GetLastError());
			gameLocal.Printf("PreySplit integration is not available.\n");
			CloseHandle(pipe_preysplit);
			pipe_preysplit = INVALID_HANDLE_VALUE;
		}
	}

	void ShutdownPreySplitPipe()
	{
		if (pipe_preysplit != INVALID_HANDLE_VALUE) {
			CloseHandle(pipe_preysplit);
			//gameLocal.Printf("Closed the PreySplit pipe.\n");
		}
		pipe_preysplit = INVALID_HANDLE_VALUE;

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
				gameLocal.Printf("WaitForSingleObject failed with %d.\n", GetLastError());
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
				gameLocal.Printf("ConnectNamedPipe failed with %d.\n", err);
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
				//gameLocal.Printf("WriteFile failed with %d.\n", err);
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

		WritePreySplit(buf);
	}

	void WriteMapChange(const Time& time, idStr& map)
	{
		// normal map path: maps/game/roadhouse.map
		// get turned into: roadhouse.map
		map.Replace("maps/game/", "");

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

		WritePreySplit(buf);
	}

	void WriteTimerStart(const Time& time)
	{
		std::vector<char> buf(11);
		buf[0] = static_cast<char>(buf.size());
		buf[1] = static_cast<char>(MessageType::EVENT);
		buf[2] = static_cast<char>(EventType::TIMER_START);
		AddTimeToBuffer(buf.data() + 3, time);

		WritePreySplit(buf);
	}

	Time GetTime()
	{
		bool isrun = pr_Timer.IsRunning();
		if (isrun)
		{
			pr_Timer.Stop();
		}

		auto ms = pr_Timer.Milliseconds();

		if (isrun)
		{
			pr_Timer.Start();
		}
		auto hour = ms / (60 * 60 * 1000);
		ms = ms - hour*(60 * 60 * 1000);
		auto minute = ms / (60 * 1000);
		ms = ms - minute*(60 * 1000);
		auto seconds = ms / 1000;
		ms = ms - seconds * 1000;

		// cast our doubles to the uints we need
		return Time{ static_cast<uint32_t>(hour),static_cast<uint8_t>(minute),static_cast<uint8_t>(seconds),static_cast<uint16_t>(ms) };
	}
}