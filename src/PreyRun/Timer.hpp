// PreyRun BEGIN
#pragma once
#include <chrono>

namespace pr
{
	class pr_Timer
	{
	public:
		pr_Timer();
		~pr_Timer();

		void start();
		void pause();
		void reset();
		unsigned long long now();
		bool isRunning();

	private:
		unsigned long long acuTime;
		unsigned long long startTime;
		bool running;

		// funktion used for the get the current time
		std::chrono::milliseconds currentTime();
	};

}
// PreyRun END