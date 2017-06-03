#pragma once

#include "StdLib.hpp"

// Created by CppPhil (https://github.com/CppPhil)

class prThread
{
public:
	template <typename Callable>
	static void launch(Callable &&callable)
	{
		new prThread { std::forward<Callable>(callable) };
	}

private:
	template <typename Callable>
	void fun(Callable &&callable)
	{
		callable();
		delete this;
	}

	template <typename Callable>
	explicit prThread(Callable &&callable)
		: m_thread { [this](auto &&callable) { fun(std::forward<decltype(callable)>(callable)); }, std::forward<Callable>(callable) }
	{
		m_thread.detach();
	}

	std::thread m_thread;
};
