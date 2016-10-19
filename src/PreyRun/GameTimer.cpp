#include "../idLib/precompiled.h"
#pragma hdrstop

#include "../game/Game_local.h"
#include "PreyRun.hpp"
#include "GameTimer.hpp"

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
#ifdef PR_DBG_GAMETIMER
	gameLocal.Printf("prTimer::Start()\n");
#endif // PR_DBG_GAMETIMER

	lastStarted = timer.now();

	isRunning = true;
}

void prTimer::Stop()
{
#ifdef PR_DBG_GAMETIMER
	gameLocal.Printf("prTimer::Stop()\n");
#endif // PR_DBG_GAMETIMER

	auto diff = timer.now() - lastStarted;
	acuTime += std::chrono::duration <storageType, periodType> (diff);

	isRunning = false;
}

void prTimer::Clear()
{
#ifdef PR_DBG_GAMETIMER
	gameLocal.Printf("prTimer::Clear()\n");
#endif // PR_DBG_GAMETIMER

	acuTime = std::chrono::duration<storageType, periodType> (0);
}

storageType prTimer::Milliseconds() const
{
	if (isRunning)
	{
		auto diff = timer.now() - lastStarted;

		diff += std::chrono::duration_cast<nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_TIME
		gameLocal.Printf("prTimer::Milliseconds diff=%d\n",std::chrono::duration_cast<millisec>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<millisec>(diff).count();
	}
#ifdef PR_DBG_GAMETIMER_GET_TIME
	gameLocal.Printf("prTimer::Milliseconds acuTime=%d\n", std::chrono::duration_cast<millisec>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

	return std::chrono::duration_cast<millisec>(acuTime).count();
}

storageType prTimer::Microseconds() const
{
	if (isRunning)
	{
		auto diff = timer.now() - lastStarted;

		diff += std::chrono::duration_cast<nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_TIME
		gameLocal.Printf("prTimer::Microseconds diff=%d\n", std::chrono::duration_cast<microsec>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<microsec>(diff).count();
	}
#ifdef PR_DBG_GAMETIMER_GET_TIME
	gameLocal.Printf("prTimer::Microseconds acuTime=%d\n", std::chrono::duration_cast<microsec>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

	return std::chrono::duration_cast<microsec>(acuTime).count();
}

storageType prTimer::Nanoseconds() const
{
	if (isRunning)
	{
		auto diff = timer.now() - lastStarted;

		diff += std::chrono::duration_cast<nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_TIME
		gameLocal.Printf("prTimer::Nanoseconds diff=%d\n", std::chrono::duration_cast<nanosec>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<nanosec>(diff).count();
	}
#ifdef PR_DBG_GAMETIMER_GET_TIME
	gameLocal.Printf("prTimer::Nanoseconds acuTime=%d\n", std::chrono::duration_cast<nanosec>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

	return std::chrono::duration_cast<nanosec>(acuTime).count();
}

storageType prTimer::ClockTicks() const
{
	if (isRunning)
	{
		auto diff = timer.now() - lastStarted;

		diff += std::chrono::duration_cast<nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_CLOCKTICKS
		gameLocal.Printf("prTimer::ClockTicks diff=%d\n", std::chrono::duration_cast<timeType>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_CLOCKTICKS

		return std::chrono::duration_cast<timeType>(diff).count();
	}
#ifdef PR_DBG_GAMETIMER_GET_CLOCKTICKS
	gameLocal.Printf("prTimer::ClockTicks acuTime=%d\n", std::chrono::duration_cast<timeType>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_CLOCKTICKS

	return std::chrono::duration_cast<timeType>(acuTime).count();
}

void prTimer::SetCT(storageType ct)
{
#ifdef PR_DBG_GAMETIMER
	gameLocal.Printf("prTimer::SetCT ct=%f\n",ct);
#endif // PR_DBG_GAMETIMER

	acuTime = static_cast<std::chrono::duration<storageType,periodType>> (ct);
}
