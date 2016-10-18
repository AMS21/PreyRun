#pragma once

#include "StdLib.hpp"

typedef double storageType;
typedef std::chrono::microseconds timeType;
typedef timeType::period periodType;

typedef std::chrono::milliseconds millisec;
typedef std::chrono::microseconds microsec;
typedef std::chrono::nanoseconds nanosec;

class prTimer
{
public:
	prTimer();
	explicit prTimer(storageType ct);
	~prTimer();

	void Start(void);
	void Stop(void);
	void Clear(void);
	storageType ClockTicks(void) const;
	storageType Milliseconds(void) const;

	bool IsRunning() const { return isRunning; }
	void SetCT(storageType ct);

private:
	bool isRunning;
	std::chrono::duration <storageType, periodType> acuTime;

	std::chrono::time_point<std::chrono::steady_clock> lastStarted;
	std::chrono::steady_clock timer;
};
