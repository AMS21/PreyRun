#pragma once

#include "PreyRun.hpp"
#include <chrono>

namespace pr
{
	void AutoStartTimers();

	void AutoStopTimers();

	namespace Timer
	{
		using nanosec = std::chrono::nanoseconds;	// 1 second = 1,000,000,000 nanoseconds
		using microsec = std::chrono::microseconds;	// 1 second = 1,000,000		microseconds
		using millisec = std::chrono::milliseconds;	// 1 second = 1,000			milliseconds

		using storage_type = double;
		using duration_type = microsec;
		using period_type = duration_type::period;

		constexpr char pr_gametimer_clocktick_postfix[] { "us" }; // postfixes: Milliseconds (ms), Microseconds (us), Nanoseconds (ns)
	}

	class prTimer
	{
	public:
		prTimer() noexcept;
		explicit prTimer(const pr::Timer::storage_type& ct) noexcept;

		void Start() noexcept;
		void Stop() noexcept;
		void Clear() noexcept;

		pr::Timer::storage_type Milliseconds() const noexcept;
		pr::Timer::storage_type Microseconds() const noexcept;
		pr::Timer::storage_type Nanoseconds() const noexcept;

		// Always returns the time represented in the underlying type. pr::timer::duration_type
		pr::Timer::storage_type ClockTicks() const noexcept;

		bool IsRunning() const noexcept { return mIsRunning; }
		void SetCT(const pr::Timer::storage_type& clockTicks) noexcept;

	private:
		bool mIsRunning;
		std::chrono::duration<pr::Timer::storage_type, pr::Timer::period_type> mAcuTime;

		std::chrono::time_point<std::chrono::high_resolution_clock> mLastStarted;
		std::chrono::high_resolution_clock mTimer;
	};
} // End of namespace: pr
