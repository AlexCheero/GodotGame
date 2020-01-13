#pragma once

#include <stdint.h>

#include <OS.hpp>

namespace utils
{
	class Timer
	{
	private:
		//TODO: use unified with or without m_
		int64_t m_startTime = -1;
	public:
		void Start(int64_t at = 0) { m_startTime = godot::OS::get_singleton()->get_ticks_msec() - at; }
		void Stop() { m_startTime = -1; }
		int64_t PassedMSec() { return godot::OS::get_singleton()->get_ticks_msec() - m_startTime; }
	};
}