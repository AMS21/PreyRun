#include "../idLib/precompiled.h"
#pragma hdrstop

#include "GameTimer.hpp"
#include "../game/Game_local.h"

prTimer::prTimer(): isRunning(false), acuTime(std::chrono::duration<storageType,periodType> (0))
{
	lastStarted = timer.now();
}

prTimer::prTimer(storageType ct) : isRunning(false), acuTime(std::chrono::duration < storageType, periodType > (ct))
{
	lastStarted = timer.now();
}

prTimer::~prTimer()
{
}

void prTimer::Start()
{
	lastStarted = timer.now();

	isRunning = true;
}

void prTimer::Stop()
{
	auto diff = timer.now() - lastStarted;
	acuTime += std::chrono::duration <storageType, periodType> (diff);

	isRunning = false;
}

void prTimer::Clear()
{
	acuTime = std::chrono::duration<storageType, periodType> (0);
}

storageType prTimer::Milliseconds() const
{
	if (isRunning)
	{
		auto diff = timer.now() - lastStarted;

		diff += std::chrono::duration_cast<nanosec> (acuTime);

		return std::chrono::duration_cast<millisec>(diff).count();
	}

	return std::chrono::duration_cast<millisec>(acuTime).count();
}

storageType prTimer::ClockTicks() const
{
	if (isRunning)
	{
		auto diff = timer.now() - lastStarted;

		diff += std::chrono::duration_cast<nanosec> (acuTime);

		return std::chrono::duration_cast<timeType>(diff).count();
	}

	return std::chrono::duration_cast<timeType>(acuTime).count();
}

void prTimer::SetCT(storageType ct)
{
	gameLocal.Printf("Setting CT to: %f\n", ct);
	acuTime = static_cast<std::chrono::duration<storageType,periodType>> (ct);
	gameLocal.Printf("ms after set: %f\n", Milliseconds());
}
