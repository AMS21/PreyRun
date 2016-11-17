#pragma once

#include "StdLib.hpp"

namespace pr
{
	namespace timer
	{
		using nanosec = std::chrono::nanoseconds;	// 1 second = 1,000,000,000 nanoseconds
		using microsec = std::chrono::microseconds;	// 1 second = 1,000,000		microseconds
		using millisec = std::chrono::milliseconds;	// 1 second = 1,000			milliseconds

		using storageType = double;
		using timeType = microsec;
		using periodType = timeType::period;

		constexpr char pr_gametimer_clocktick_postfix[]{ "us" }; // postfixes: Milliseconds (ms), Microseconds (us), Nanoseconds (ns)
	}
}

class prTimer
{
public:
	prTimer();
	explicit prTimer(pr::timer::storageType ct);
	~prTimer();

	void Start();
	void Stop();
	void Clear();

	pr::timer::storageType Milliseconds() const;
	pr::timer::storageType Microseconds() const;
	pr::timer::storageType Nanoseconds() const;

	// Always returns the time represented in the underlying type. pr::timer::timeType
	pr::timer::storageType ClockTicks() const;

	bool IsRunning() const { return isRunning; }
	void SetCT(pr::timer::storageType clockTicks);

private:
	bool isRunning;
	std::chrono::duration <pr::timer::storageType, pr::timer::periodType> acuTime;

	std::chrono::time_point<std::chrono::steady_clock> lastStarted;
	std::chrono::steady_clock timer;
};
