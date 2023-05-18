#pragma once

namespace Engine
{
	typedef void* ThreadId;

	bool InMainThread();

	ThreadId GetCurrentThreadId();

} // namespace Engine
