#pragma warning(disable:4312)

#include "Threading.h"

#include <Windows.h>

namespace Engine
{
	ThreadId GetCurrentThreadId()
	{
		return reinterpret_cast<ThreadId>(::GetCurrentThreadId());
	}

} // Engine