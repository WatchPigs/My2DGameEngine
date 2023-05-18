#pragma once

#include "Target.h"

#include <inttypes.h>

#if _WIN64
VERIFY_64BIT;
#define HEXPRINTCHARS 16
#else
VERIFY_32BIT;
#define HEXPRINTCHARS 8
#endif // _64BIT

// Handy C preprocessor macros to convert text to quoted strings
#define AS_STRING(str) #str
#define TO_STRING(def) AS_STRING(def)

#define PRETTY_PTR_HEX "0x0" TO_STRING(HEXPRINTCHARS) "%" PRIxPTR

namespace Engine
{
	void ConsolePrint(const char* i_fmt, ...);
} // Namespace Engine

// This is how we handle variable argument pre-processor macros
// Supported by compilers that support the C99 standard
#if defined(_DEBUG)  &&  !defined(DISABLE_DEBUG_PRINT)
#define DEBUG_PRINT(fmt,...) Engine::ConsolePrint((fmt),__VA_ARGS__)
#else
// This is one way we create a do nothing (NO OP) macro that doesn't
// generate a compiler warning or error
#define DEBUG_PRINT(fmt,...) void(0)
#endif