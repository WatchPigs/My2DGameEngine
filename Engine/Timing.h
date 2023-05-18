#pragma once
#include <stdint.h>

namespace Engine
{
	namespace Timing
	{
		typedef uint64_t	tick_t;

		bool Init();

		tick_t GetCurrentTickCounter();
		float GetTimeDiff_ms(tick_t i_StartTick, tick_t i_EndTick = GetCurrentTickCounter());
		float CvtTicksToMilliseconds(tick_t i_TickCount);

		inline float CvtSecondsToMilliseconds(float i_Seconds);
		inline float CvtPerSecondToPerMillisecond(float i_PerSecond);
	}
}
