#pragma once

#include "StdLib.hpp"

namespace pr
{
	namespace Timer
	{
		using nanosec = std::chrono::nanoseconds;	// 1 second = 1,000,000,000 nanoseconds
		using microsec = std::chrono::microseconds;	// 1 second = 1,000,000		microseconds
		using millisec = std::chrono::milliseconds;	// 1 second = 1,000			milliseconds

		using storageType = double;
		using timeType = microsec;
		using periodType = timeType::period;

		constexpr char pr_gametimer_clocktick_postfix[] { "us" }; // postfixes: Milliseconds (ms), Microseconds (us), Nanoseconds (ns)
	}

	class prTimer
	{
	public:
		prTimer();
		explicit prTimer(pr::Timer::storageType ct);
		~prTimer();

		void Start();
		void Stop();
		void Clear();

		pr::Timer::storageType Milliseconds() const;
		pr::Timer::storageType Microseconds() const;
		pr::Timer::storageType Nanoseconds() const;

		// Always returns the time represented in the underlying type. pr::timer::timeType
		pr::Timer::storageType ClockTicks() const;

		bool IsRunning() const { return isRunning; }
		void SetCT(pr::Timer::storageType clockTicks);

	private:
		bool isRunning;
		std::chrono::duration <pr::Timer::storageType, pr::Timer::periodType> acuTime;

		std::chrono::time_point<std::chrono::steady_clock> lastStarted;
		std::chrono::steady_clock timer;
	};
}
