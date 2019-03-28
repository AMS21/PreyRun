#pragma once

#include <cstdint>

namespace pr
{
	struct Time
	{
		uint32_t hours;
		uint8_t minutes;
		uint8_t seconds;
		uint16_t milliseconds;
	};

	Time ms2time(unsigned ms) noexcept;

	PR_FINLINE idStr ms2string(const unsigned ms)
	{
		auto const time = ms2time(ms);

		idStr str;
		sprintf(str, "%02d:%02d:%02d.%03d", time.hours, time.minutes, time.seconds, time.milliseconds);

		return idStr(std::move(str));
	}

	// The draw functions from Prey takes floats in the range from 0.0 to 1.0 but usually colors are represented from 0 to 255 (hex), this is just a handy function which does the conversion for us
	PR_FINLINE constexpr float hexToFloat(const int val) noexcept
	{
		return static_cast<float>(val) / 255.f;
	}

	idStr formatTimeString(const Time& time, const bool allDigits, const uint8_t& msPrecision);

	idVec4 AddAlphaValue(const idVec3& original, const float alpha);

	constexpr bool string_equals(const char* str1, const char* str2)
	{
		while (*str1 != '\0')
		{
			if (*str1 != *str2)
			{
				return false;
			}

			++str1;
			++str2;
		}

		return *str1 == *str2;
	}
} // End of namespace: pr
