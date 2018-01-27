#pragma once

#include "GameTimer.hpp"

#include <mutex>

// Created by CppPhil (https://github.com/CppPhil)

/*!
* \brief Stores shared data in its private section.
*        Allows different threads to operate on the shared data
*        by passing in callables that operate on the shared data.
**/
template <typename SharedData>
class Monitor final
{
public:
	using this_type = Monitor;
	using element_type = SharedData;

	/*!
	* \brief Creates a Monitor.
	* \param sharedData the data to be protected by the Monitor.
	**/
	explicit Monitor(element_type sharedData) noexcept
		: m_sharedData { std::move(sharedData) },
		m_mutex {}
	{}

	/*!
	* \brief Receives a callable and invokes that callable by passing the
	*        shared data to it. The call itself is protected by a mutex.
	* \param callable The callable to be used to operate on the shared data.
	* \return The result of calling the callable passed in with the shared data
	*         as the callable's call operator's argument.
	**/
	template <typename Callable>
	auto operator()(Callable &&callable) const -> decltype(auto)
	{
		std::lock_guard<std::mutex> lockGuard { m_mutex };
		return std::invoke(std::forward<Callable>(callable), m_sharedData);
	}

private:
	mutable element_type m_sharedData; //!< the shared data
	mutable std::mutex m_mutex; /*!< the mutex to guard access
								*   to the shared data
								**/
};

// Wrapper Object

struct prTimerMonitor
{
public:
	prTimerMonitor() noexcept : m_monitor(pr::prTimer())
	{};

	void Start() { m_monitor(&pr::prTimer::Start); }
	void Stop() { m_monitor(&pr::prTimer::Stop); }
	void Clear() { m_monitor(&pr::prTimer::Clear); }

	pr::Timer::storage_type Milliseconds() { return m_monitor(&pr::prTimer::Milliseconds); }
	pr::Timer::storage_type Microseconds() { return m_monitor(&pr::prTimer::Microseconds); }
	pr::Timer::storage_type Nanoseconds() { return m_monitor(&pr::prTimer::Nanoseconds); }

	pr::Timer::storage_type ClockTicks() { return m_monitor(&pr::prTimer::ClockTicks); }

	bool IsRunning() { return m_monitor(&pr::prTimer::IsRunning); }
	void SetCT(int clockTicks) noexcept
	{
		m_monitor([&clockTicks](pr::prTimer &myType) { myType.SetCT(clockTicks); });
	}

	Monitor<pr::prTimer> m_monitor;
};
