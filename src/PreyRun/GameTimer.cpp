#include "../idLib/precompiled.h"
#pragma hdrstop

#include "../game/Game_local.h"
#include "PreyRun.hpp"
#include "GameTimer.hpp"

namespace pr
{
	prTimer::prTimer() : isRunning(false), acuTime(std::chrono::duration<pr::Timer::storageType, pr::Timer::periodType>(0)), lastStarted(timer.now()) {}

	prTimer::prTimer(const pr::Timer::storageType& ct) : isRunning(false), acuTime(std::chrono::duration <pr::Timer::storageType, pr::Timer::periodType>(ct)), lastStarted(timer.now()) {}

	prTimer::~prTimer() {}

	void prTimer::Start()
	{
#ifdef PR_DBG_GAMETIMER
		pr::ConsoleWrite("prTimer::Start()");
#endif // PR_DBG_GAMETIMER

		lastStarted = timer.now();

		isRunning = true;
	}

	void prTimer::Stop()
	{
#ifdef PR_DBG_GAMETIMER
		pr::ConsoleWrite("prTimer::Start()");
#endif // PR_DBG_GAMETIMER

		if (!isRunning) { return; }

		auto diff = timer.now() - lastStarted;
		acuTime += std::chrono::duration <pr::Timer::storageType, pr::Timer::periodType>(diff);

		isRunning = false;
	}

	void prTimer::Clear()
	{
#ifdef PR_DBG_GAMETIMER
		pr::ConsoleWrite("prTimer::Clear()");
#endif // PR_DBG_GAMETIMER

		acuTime = std::chrono::duration<pr::Timer::storageType, pr::Timer::periodType>(0);
	}

	pr::Timer::storageType prTimer::Milliseconds() const
	{
		if (isRunning)
		{
			auto diff = timer.now() - lastStarted;

			diff += std::chrono::duration_cast<pr::Timer::nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_TIME
			pr::FunctionLog(__FUNCTION__, "diff: %d", std::chrono::duration_cast<pr::Timer::millisec>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

			return std::chrono::duration_cast<pr::Timer::millisec>(diff).count();
		}
#ifdef PR_DBG_GAMETIMER_GET_TIME
		pr::FunctionLog(__FUNCTION__, "acuTime: %d", std::chrono::duration_cast<pr::Timer::millisec>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<pr::Timer::millisec>(acuTime).count();
	}

	pr::Timer::storageType prTimer::Microseconds() const
	{
		if (isRunning)
		{
			auto diff = timer.now() - lastStarted;

			diff += std::chrono::duration_cast<pr::Timer::nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_TIME
			pr::FunctionLog(__FUNCTION__, "diff: %d", std::chrono::duration_cast<pr::Timer::millisec>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

			return std::chrono::duration_cast<pr::Timer::microsec>(diff).count();
		}
#ifdef PR_DBG_GAMETIMER_GET_TIME
		pr::FunctionLog(__FUNCTION__, "acuTime: %d", std::chrono::duration_cast<pr::Timer::millisec>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<pr::Timer::microsec>(acuTime).count();
	}

	pr::Timer::storageType prTimer::Nanoseconds() const
	{
		if (isRunning)
		{
			auto diff = timer.now() - lastStarted;

			diff += std::chrono::duration_cast<pr::Timer::nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_TIME
			pr::FunctionLog(__FUNCTION__, "diff: %d", std::chrono::duration_cast<pr::Timer::millisec>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

			return std::chrono::duration_cast<pr::Timer::nanosec>(diff).count();
		}
#ifdef PR_DBG_GAMETIMER_GET_TIME
		pr::FunctionLog(__FUNCTION__, "acuTime: %d", std::chrono::duration_cast<pr::Timer::millisec>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<pr::Timer::nanosec>(acuTime).count();
	}

	pr::Timer::storageType prTimer::ClockTicks() const
	{
		if (isRunning)
		{
			auto diff = timer.now() - lastStarted;

			diff += std::chrono::duration_cast<pr::Timer::nanosec> (acuTime);

#ifdef PR_DBG_GAMETIMER_GET_CLOCKTICKS
			pr::FunctionLog(__FUNCTION__, "diff: %d", std::chrono::duration_cast<pr::Timer::millisec>(diff).count());
#endif // PR_DBG_GAMETIMER_GET_CLOCKTICKS

			return std::chrono::duration_cast<pr::Timer::timeType>(diff).count();
		}
#ifdef PR_DBG_GAMETIMER_GET_CLOCKTICKS
		pr::FunctionLog(__FUNCTION__, "acuTime: %d", std::chrono::duration_cast<pr::Timer::millisec>(acuTime).count());
#endif // PR_DBG_GAMETIMER_GET_CLOCKTICKS

		return std::chrono::duration_cast<pr::Timer::timeType>(acuTime).count();
	}

	void prTimer::SetCT(const pr::Timer::storageType& clockTicks)
	{
#ifdef PR_DBG_GAMETIMER
		pr::FunctionLog(__FUNCTION__, "ct: %f", clockTicks);
#endif // PR_DBG_GAMETIMER

		acuTime = static_cast<std::chrono::duration<pr::Timer::storageType, pr::Timer::periodType>> (clockTicks);
	}
}
