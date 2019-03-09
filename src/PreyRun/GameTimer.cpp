#include "../idLib/precompiled.h"
#pragma hdrstop

#include "GameTimer.hpp"

#include "Constants.hpp"
#include "Cvar.hpp"
#include "Ghosting/GhostRecord.hpp"
#include "Interprocess.hpp"
#include "Logging.hpp"
#include "Utility.hpp"

#include "../idLib/Str.h"

namespace pr
{
	void AutoStartTimers()
	{
		// Reset all timers
		pr::Timer::inGame.Clear();
		pr::Timer::RTA.Clear();

		pr::Timer::inGame.Start();
		pr::Timer::RTA.Start();

		pr::Timer::running = true;
		pr::runFinished = false;

		if (pr::Cvar::preysplit.GetBool())
		{
			pr::WriteTimerStart(pr::GetTime());
		}

		if (pr::Cvar::exec_runconfig.GetBool())
		{
			if (static_cast<pr::TimerMethode>(pr::Cvar::timer_methode.GetInteger()) == pr::TimerMethode::RealTimeAttack)
			{
				cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "exec RTARun.cfg\n");
			}
			else if (static_cast<pr::TimerMethode>(pr::Cvar::timer_methode.GetInteger()) == pr::TimerMethode::IndividualLevel)
			{
				cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "exec ILRun.cfg\n");
			}
		}

		pr::Log("Timer: Auto starting");
	}

	void AutoStopTimers()
	{
		pr::Timer::inGame.Stop();
		pr::Timer::RTA.Stop();
		pr::Timer::running = false;
		pr::runFinished = true;

		if (pr::Cvar::preysplit.GetBool())
		{
			pr::WriteGameEnd(pr::GetTime());
		}

		pr::Log("Timer: End game, game time: %s", ms2string(pr::Timer::inGame.Milliseconds()).c_str());
		pr::Log("Timer: End game, RTA time: %s", ms2string(pr::Timer::RTA.Milliseconds()).c_str());
	}

	prTimer::prTimer() noexcept
		: mIsRunning(false), mAcuTime(std::chrono::duration<pr::Timer::storage_type, pr::Timer::period_type>(0)), mLastStarted(mTimer.now())
	{}

	prTimer::prTimer(const pr::Timer::storage_type& ct) noexcept
		: mIsRunning(false), mAcuTime(std::chrono::duration <pr::Timer::storage_type, pr::Timer::period_type>(ct)), mLastStarted(mTimer.now())
	{}

	void prTimer::Start() noexcept
	{
	#ifdef PR_DBG_GAMETIMER
		pr::ConsoleWrite("prTimer::Start()");
	#endif // PR_DBG_GAMETIMER

		mLastStarted = mTimer.now();

		mIsRunning = true;
	}

	void prTimer::Stop() noexcept
	{
	#ifdef PR_DBG_GAMETIMER
		pr::ConsoleWrite("prTimer::Start()");
	#endif // PR_DBG_GAMETIMER

		if (!mIsRunning) { return; }

		const auto diff = mTimer.now() - mLastStarted;
		mAcuTime += std::chrono::duration <pr::Timer::storage_type, pr::Timer::period_type>(diff);

		mIsRunning = false;
	}

	void prTimer::Clear() noexcept
	{
	#ifdef PR_DBG_GAMETIMER
		pr::ConsoleWrite("prTimer::Clear()");
	#endif // PR_DBG_GAMETIMER

		mAcuTime = std::chrono::duration<pr::Timer::storage_type, pr::Timer::period_type>(0);
	}

	pr::Timer::storage_type prTimer::Milliseconds() const noexcept
	{
		if (mIsRunning)
		{
			auto diff = mTimer.now() - mLastStarted;

			diff += std::chrono::duration_cast<pr::Timer::nanosec> (mAcuTime);

		#ifdef PR_DBG_GAMETIMER_GET_TIME
			pr::FunctionLog(__FUNCTION__, "diff: %d", std::chrono::duration_cast<pr::Timer::millisec>(diff).count());
		#endif // PR_DBG_GAMETIMER_GET_TIME

			return std::chrono::duration_cast<pr::Timer::millisec>(diff).count();
		}
	#ifdef PR_DBG_GAMETIMER_GET_TIME
		pr::FunctionLog(__FUNCTION__, "acuTime: %d", std::chrono::duration_cast<pr::Timer::millisec>(acuTime).count());
	#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<pr::Timer::millisec>(mAcuTime).count();
	}

	pr::Timer::storage_type prTimer::Microseconds() const noexcept
	{
		if (mIsRunning)
		{
			auto diff = mTimer.now() - mLastStarted;

			diff += std::chrono::duration_cast<pr::Timer::nanosec> (mAcuTime);

		#ifdef PR_DBG_GAMETIMER_GET_TIME
			pr::FunctionLog(__FUNCTION__, "diff: %d", std::chrono::duration_cast<pr::Timer::millisec>(diff).count());
		#endif // PR_DBG_GAMETIMER_GET_TIME

			return std::chrono::duration_cast<pr::Timer::microsec>(diff).count();
		}
	#ifdef PR_DBG_GAMETIMER_GET_TIME
		pr::FunctionLog(__FUNCTION__, "acuTime: %d", std::chrono::duration_cast<pr::Timer::millisec>(acuTime).count());
	#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<pr::Timer::microsec>(mAcuTime).count();
	}

	pr::Timer::storage_type prTimer::Nanoseconds() const noexcept
	{
		if (mIsRunning)
		{
			auto diff = mTimer.now() - mLastStarted;

			diff += std::chrono::duration_cast<pr::Timer::nanosec> (mAcuTime);

		#ifdef PR_DBG_GAMETIMER_GET_TIME
			pr::FunctionLog(__FUNCTION__, "diff: %d", std::chrono::duration_cast<pr::Timer::millisec>(diff).count());
		#endif // PR_DBG_GAMETIMER_GET_TIME

			return std::chrono::duration_cast<pr::Timer::nanosec>(diff).count();
		}
	#ifdef PR_DBG_GAMETIMER_GET_TIME
		pr::FunctionLog(__FUNCTION__, "acuTime: %d", std::chrono::duration_cast<pr::Timer::millisec>(acuTime).count());
	#endif // PR_DBG_GAMETIMER_GET_TIME

		return std::chrono::duration_cast<pr::Timer::nanosec>(mAcuTime).count();
	}

	pr::Timer::storage_type prTimer::ClockTicks() const noexcept
	{
		if (mIsRunning)
		{
			auto diff = mTimer.now() - mLastStarted;

			diff += std::chrono::duration_cast<pr::Timer::nanosec> (mAcuTime);

		#ifdef PR_DBG_GAMETIMER_GET_CLOCKTICKS
			pr::FunctionLog(__FUNCTION__, "diff: %d", std::chrono::duration_cast<pr::Timer::millisec>(diff).count());
		#endif // PR_DBG_GAMETIMER_GET_CLOCKTICKS

			return std::chrono::duration_cast<pr::Timer::duration_type>(diff).count();
		}
	#ifdef PR_DBG_GAMETIMER_GET_CLOCKTICKS
		pr::FunctionLog(__FUNCTION__, "acuTime: %d", std::chrono::duration_cast<pr::Timer::millisec>(acuTime).count());
	#endif // PR_DBG_GAMETIMER_GET_CLOCKTICKS

		return std::chrono::duration_cast<pr::Timer::duration_type>(mAcuTime).count();
	}

	void prTimer::SetCT(const pr::Timer::storage_type& clockTicks) noexcept
	{
	#ifdef PR_DBG_GAMETIMER
		pr::FunctionLog(__FUNCTION__, "ct: %f", clockTicks);
	#endif // PR_DBG_GAMETIMER

		mAcuTime = static_cast<std::chrono::duration<pr::Timer::storage_type, pr::Timer::period_type>> (clockTicks);
	}
} // End of namespace: pr
