#pragma once

#include "StdLib.hpp"

typedef std::chrono::nanoseconds nanosec;   // 1 second = 1,000,000,000 nanoseconds
typedef std::chrono::microseconds microsec; // 1 second = 1,000,000		microseconds
typedef std::chrono::milliseconds millisec; // 1 second = 1,000			milliseconds

typedef double storageType;
typedef microsec timeType;
typedef timeType::period periodType;

constexpr char * pr_gametimer_clocktick_post{ "us" };

class prTimer
{
public:
	prTimer();
	explicit prTimer(storageType ct);
	~prTimer();

	void Start();
	void Stop();
	void Clear();

	storageType Milliseconds() const;
	storageType Microseconds() const;
	storageType Nanoseconds() const;

	storageType ClockTicks() const;

	bool IsRunning() const { return isRunning; }
	void SetCT(storageType ct);

private:
	bool isRunning;
	std::chrono::duration <storageType, periodType> acuTime;

	std::chrono::time_point<std::chrono::steady_clock> lastStarted;
	std::chrono::steady_clock timer;
};
