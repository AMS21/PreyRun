#include "../idLib/precompiled.h"
#pragma hdrstop

#include "Utility.hpp"

namespace pr
{
	Time ms2time(unsigned ms) noexcept
	{
		Time ts {};

		ts.hours = ms / (60 * 60 * 1'000);
		ms = ms - ts.hours * (60 * 60 * 1'000);
		ts.minutes = ms / (60 * 1'000);
		ms = ms - ts.minutes * (60 * 1'000);
		ts.seconds = ms / 1'000;
		ts.milliseconds = ms - ts.seconds * 1'000;

		return ts;
	}

	idStr formatTimeString(const Time& time, const bool allDigits, const uint8_t& msPrecision)
	{
		idStr retStr;

		if (allDigits)
		{
			sprintf(retStr, "%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
		}
		else
		{
			if (time.hours != 0)
			{
				sprintf(retStr, "%d:%02d:%02d", time.hours, time.minutes, time.seconds);
			}
			else
			{
				if (time.minutes != 0)
				{
					sprintf(retStr, "%d:%02d", time.minutes, time.seconds);
				}
				else
				{
					sprintf(retStr, "%d", time.seconds);
				}
			}
		}

		// Add milliseconds if needed
		if (msPrecision != 0)
		{
			idStr tempStr;

			// 500 ms = 0.500
			sprintf(tempStr, "%.*f", msPrecision, static_cast<float>(time.milliseconds) / 1'000.00f);

			// Add the string but without the 0 in front
			// 500 ms = .500
			retStr.Append(tempStr.Mid(1, tempStr.Length()));
		}

		return retStr;
	}

	idVec4 AddAlphaValue(const idVec3& original, const float alpha)
	{
		return idVec4(original.x, original.y, original.z, alpha);
	}
} // End of namespace: pr
