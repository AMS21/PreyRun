#include "../idLib/precompiled.h"
#pragma hdrstop

#include "../game/Game_local.h"
#include "PreyRun.hpp"
#include "GameTimer.hpp"

prTimer::prTimer() : isRunning(false), acuTime(std::chrono::duration<pr::timer::storageType, pr::timer::periodType>(0)), lastStarted(timer.now()) {}

prTimer::prTimer(pr::timer::storageType ct) : isRunning(false), acuTime(std::chrono::duration <pr::timer::storageType, pr::timer::periodType>(ct)), lastStarted(timer.now()) {}

prTimer::~prTimer() {}

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

	if (!isRunning) { return; }

	auto diff = timer.now() - lastStarted;
	acuTime += std::chrono::duration <pr::timer::storageType, pr::timer::periodType>(diff);

	isRunning = false;
}

void prTimer::Clear()
{
#ifdef PR_DBG_GAMETIMER
	gameLocal.Printf("prTimer::Clear()\n");
#endif // PR_DBG_GAMETIMER

	acuTime = std::chrono::duration<pr::timer::storageType, pr::timer::periodType>(0);
}

pr::timer::storageType prTimer::Milliseconds() const
{
	if (isRunning)
	{
		auto diff = timer.now() - lastStarted;

		diff += std::chrono::duration_cast<pr::timer::nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_TIME
		gameLocal.Printf("prTimer::Milliseconds diff=%d\n", std::chrono::duration_cast<pr::timer::millisec>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<pr::timer::millisec>(diff).count();
	}
#ifdef PR_DBG_GAMETIMER_GET_TIME
	gameLocal.Printf("prTimer::Milliseconds acuTime=%d\n", std::chrono::duration_cast<pr::timer::millisec>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

	return std::chrono::duration_cast<pr::timer::millisec>(acuTime).count();
}

pr::timer::storageType prTimer::Microseconds() const
{
	if (isRunning)
	{
		auto diff = timer.now() - lastStarted;

		diff += std::chrono::duration_cast<pr::timer::nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_TIME
		gameLocal.Printf("prTimer::Microseconds diff=%d\n", std::chrono::duration_cast<pr::timer::microsec>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<pr::timer::microsec>(diff).count();
	}
#ifdef PR_DBG_GAMETIMER_GET_TIME
	gameLocal.Printf("prTimer::Microseconds acuTime=%d\n", std::chrono::duration_cast<pr::timer::microsec>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

	return std::chrono::duration_cast<pr::timer::microsec>(acuTime).count();
}

pr::timer::storageType prTimer::Nanoseconds() const
{
	if (isRunning)
	{
		auto diff = timer.now() - lastStarted;

		diff += std::chrono::duration_cast<pr::timer::nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_TIME
		gameLocal.Printf("prTimer::Nanoseconds diff=%d\n", std::chrono::duration_cast<pr::timer::nanosec>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<pr::timer::nanosec>(diff).count();
	}
#ifdef PR_DBG_GAMETIMER_GET_TIME
	gameLocal.Printf("prTimer::Nanoseconds acuTime=%d\n", std::chrono::duration_cast<pr::timer::nanosec>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

	return std::chrono::duration_cast<pr::timer::nanosec>(acuTime).count();
}

pr::timer::storageType prTimer::ClockTicks() const
{
	if (isRunning)
	{
		auto diff = timer.now() - lastStarted;

		diff += std::chrono::duration_cast<pr::timer::nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_CLOCKTICKS
		gameLocal.Printf("prTimer::ClockTicks diff=%d\n", std::chrono::duration_cast<pr::timer::timeType>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_CLOCKTICKS

		return std::chrono::duration_cast<pr::timer::timeType>(diff).count();
	}
#ifdef PR_DBG_GAMETIMER_GET_CLOCKTICKS
	gameLocal.Printf("prTimer::ClockTicks acuTime=%d\n", std::chrono::duration_cast<pr::timer::timeType>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_CLOCKTICKS

	return std::chrono::duration_cast<pr::timer::timeType>(acuTime).count();
}

void prTimer::SetCT(pr::timer::storageType clockTicks)
{
#ifdef PR_DBG_GAMETIMER
	gameLocal.Printf("prTimer::SetCT ct=%f\n", clockTicks);
#endif // PR_DBG_GAMETIMER

	acuTime = static_cast<std::chrono::duration<pr::timer::storageType, pr::timer::periodType>> (clockTicks);
}
