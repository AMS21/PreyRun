// PreyRun BEGIN

#include "Timer.hpp"
#include <chrono>

namespace pr
{
	pr_Timer::pr_Timer()
	{
		this->startTime = 0;
		this->acuTime = 0;
		this->running = false;
	}


	pr_Timer::~pr_Timer()
	{
	}

	void pr_Timer::start()
	{
		this->running = true;
		this->startTime = currentTime().count();
	}

	void pr_Timer::pause()
	{
		this->running = false;
		this->acuTime += currentTime().count() - this->startTime;
		this->startTime = 0;
	}

	void pr_Timer::reset()
	{
		this->running = false;
		this->acuTime = 0;
		this->startTime = 0;
	}

	unsigned long long pr_Timer::now()
	{
		return acuTime + (currentTime().count() - this->startTime);
	}

	bool pr_Timer::isRunning()
	{
		return this->running;
	}

	// Private

	std::chrono::milliseconds pr_Timer::currentTime()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	}

}
// PreyRun END