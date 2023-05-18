#include <Windows.h>

namespace Engine
{
	bool IsDebuggerPresent()
	{
		return ::IsDebuggerPresent();
	}

	void BreakInDebugger()
	{
		DebugBreak();
	}
}