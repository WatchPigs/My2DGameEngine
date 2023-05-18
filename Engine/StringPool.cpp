#include <assert.h>

#include "StringPool.h"

namespace Engine
{
	StringPool::StringPool(void* i_pPoolMemory, size_t i_PoolBytes) :
		m_pPoolStart(reinterpret_cast<uint8_t*>(i_pPoolMemory)),
		m_pPoolFree(reinterpret_cast<uint8_t*>(i_pPoolMemory)),
		m_PoolBytes(i_PoolBytes)
	{
		assert(i_pPoolMemory);
		assert(i_PoolBytes > sizeof(uint16_t) + 1);

		add(NULL, 0);
	}

	StringPool::~StringPool()
	{
		if (m_pPoolStart)
		{
			delete[] m_pPoolStart;
			m_pPoolStart = NULL;
		}
	}

	StringPool* StringPool::Create(size_t i_PoolSizeBytes)
	{
		assert(i_PoolSizeBytes > 0);

		uint8_t* pPoolMemory = new uint8_t[i_PoolSizeBytes];
		if (pPoolMemory)
			return new StringPool(pPoolMemory, i_PoolSizeBytes);
		else
			return NULL;
	}

	const char* StringPool::find(const char* i_pSource)
	{
		assert(i_pSource);

		size_t length = strlen(i_pSource);
		assert(length < UINT16_MAX);

		uint8_t* pCurrent = m_pPoolStart;

		while (pCurrent < m_pPoolFree)
		{
			uint16_t lengthCurrent = *reinterpret_cast<uint16_t*&>(pCurrent)++;

			if (length == lengthCurrent)
			{
				if (memcmp(pCurrent, i_pSource, length) == 0)
					return reinterpret_cast<char*>(pCurrent);
			}
			pCurrent += size_t(lengthCurrent) + 1;
			// round it up to multiple of 2
			pCurrent += reinterpret_cast<uintptr_t>(pCurrent) & 1;
		}

		return add(i_pSource, length);
	}

	const char* StringPool::add(const char* i_pString, size_t i_length)
	{
		assert((m_pPoolFree + i_length + 1) < (m_pPoolStart + m_PoolBytes));
		assert(i_length < UINT16_MAX);

		*reinterpret_cast<uint16_t*&>(m_pPoolFree)++ = uint16_t(i_length);

		if (i_length)
		{
			assert(i_pString);
			memcpy(m_pPoolFree, i_pString, i_length);
		}

		m_pPoolFree[i_length] = 0; // terminator

		const char* pAddedString = reinterpret_cast<char*>(m_pPoolFree);

		m_pPoolFree += i_length + 1;
		// round it up to multiple of 2
		m_pPoolFree += reinterpret_cast<uintptr_t>(m_pPoolFree) & 1;

		return pAddedString;
	}

	StringPool& GlobalStringPool()
	{
		static const size_t StringPoolSize = 1024;

		static StringPool* Singleton = nullptr;
		if (Singleton == nullptr)
		{
			Singleton = StringPool::Create(StringPoolSize);
		}

		assert(Singleton);

		return *Singleton;
	}

}
