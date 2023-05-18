#include "Threading.h"

#include "StartupShutdown.h"

#include <functional>

namespace Engine
{
	ThreadId MainThreadId;

	bool InMainThread()
	{
		return GetCurrentThreadId() == MainThreadId;
	}

	void Init()
	{
		MainThreadId = GetCurrentThreadId();
	}

	Bootstrapper ThreadingBootstrapper(std::bind(Init), std::function<void()>());
} // namespace Engine
