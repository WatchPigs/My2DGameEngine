#pragma once
#include <assert.h>

#define VERIFY_64BIT static_assert(sizeof(void*) == 8, "This code designed for 64-bit compilation.")
#define VERIFY_32BIT static_assert(sizeof(void*) == 4, "This code designed for 32-bit compilation.")

namespace Engine
{
	bool IsDebuggerPresent();
	void BreakInDebugger();
}