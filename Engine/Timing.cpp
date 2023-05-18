#include "Timing.h"
#include "StartupShutdown.h"

#include <windows.h>
#include <assert.h>
#include <limits.h>
#include <functional>


namespace Engine
{
	namespace Timing
	{
		unsigned int LastFrame_ms;

		LARGE_INTEGER PerformanceFrequency = { 0 };

		bool Init()
		{
			if (!QueryPerformanceFrequency(&PerformanceFrequency))
				return false;

			return true;
		}

		tick_t GetCurrentTickCounter()
		{
			LARGE_INTEGER CurrentFrameCounter;

			BOOL success = QueryPerformanceCounter(&CurrentFrameCounter);
			assert(success == TRUE);

			return CurrentFrameCounter.QuadPart;
		}

		float GetTimeDiff_ms(tick_t i_StartTick, tick_t i_EndTick)
		{
			assert(PerformanceFrequency.QuadPart != 0);

			return (1000.0f * static_cast<float>(i_EndTick - i_StartTick)) / PerformanceFrequency.QuadPart;
		}

		float CvtTicksToMilliseconds(tick_t i_TickCount)
		{
			assert(PerformanceFrequency.QuadPart != 0);

			return (1000.0f * static_cast<float>(i_TickCount)) / PerformanceFrequency.QuadPart;
		}

		inline float CvtSecondsToMilliseconds(float i_Seconds)
		{
			return i_Seconds * 1000.0f;
		}

		inline float CvtPerSecondToPerMillisecond(float i_PerSecond)
		{
			return i_PerSecond / 1000.0f;
		}
		Bootstrapper TimingBootstrapper(std::bind(Init), std::function<void()>());
	}
}