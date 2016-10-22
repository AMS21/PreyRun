#pragma once

#include "StdLib.hpp"

namespace pr
{
	using nanosec = std::chrono::nanoseconds;	// 1 second = 1,000,000,000 nanoseconds
	using microsec = std::chrono::microseconds;	// 1 second = 1,000,000		microseconds
	using millisec = std::chrono::milliseconds;	// 1 second = 1,000			milliseconds

	using storageType = double;
	using timeType = microsec;
	using periodType = timeType::period;

	constexpr char pr_gametimer_clocktick_post[]{ "us" };
}

class prTimer
{
public:
	prTimer();
	explicit prTimer(pr::storageType ct);
	~prTimer();

	void Start();
	void Stop();
	void Clear();

	pr::storageType Milliseconds() const;
	pr::storageType Microseconds() const;
	pr::storageType Nanoseconds() const;

	pr::storageType ClockTicks() const;

	bool IsRunning() const { return isRunning; }
	void SetCT(pr::storageType ct);

private:
	bool isRunning;
	std::chrono::duration <pr::storageType, pr::periodType> acuTime;

	std::chrono::time_point<std::chrono::steady_clock> lastStarted;
	std::chrono::steady_clock timer;
};
