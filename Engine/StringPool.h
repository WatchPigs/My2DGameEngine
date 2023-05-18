#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace Engine
{
	class StringPool
	{
	public:
		static StringPool* Create(size_t i_PoolBytes);
		~StringPool();

		const char* find(const char* i_Source);

	private:
		StringPool(void* i_pPoolMemory, size_t i_PoolBytes);

		StringPool(const StringPool& i_other) = delete;
		StringPool& operator=(const StringPool& i_other) = delete;

		const char* add(const char* i_pString, size_t i_length);

		uint8_t* m_pPoolStart;
		uint8_t* m_pPoolFree;

		size_t m_PoolBytes;
	};

	StringPool& GlobalStringPool();

} // namespace Engine
