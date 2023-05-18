#include <assert.h>
#include <string.h>
#include <type_traits>

namespace Engine
{
	constexpr HashedString::HashedString() :
		m_Hash(Hash(""))
	{
	}

	constexpr HashedString::HashedString(const char* i_string) :
		m_Hash(Hash(i_string))
#ifdef DEBUG_KEEP_STRING
		, m_pString(strdup(i_string))
#endif
	{
	}

	inline HashedString::HashedString(const HashedString& i_other) :
		m_Hash(i_other.m_Hash)
#ifdef DEBUG_KEEP_STRING
		, m_pString(strdup(i_other.m_pString))
#endif
	{
	}

	inline HashedString::~HashedString()
	{
#ifdef DEBUG_KEEP_STRING
		if (m_pString)
			free(m_pString)
#endif

	}

	inline HashedString& HashedString::operator=(const HashedString& i_other)
	{
		m_Hash = i_other.m_Hash;

#ifdef DEBUG_KEEP_STRING
		if (m_pString)
			free(m_pString)

			m_pString = i_other.m_pString;
#endif

		return *this;
	}

	inline size_t HashedString::get(void) const
	{
		return m_Hash;
	}

	inline bool HashedString::operator==(const HashedString& i_other) const
	{
		return m_Hash == i_other.m_Hash;
	}

	constexpr size_t HashedString::Hash(const char* i_string)
	{
		assert(i_string);

		return Hash(reinterpret_cast<void*>(const_cast<char*>(i_string)), strlen(i_string));
	}

#ifdef _WIN64
	constexpr size_t HashedString::Hash(const void* i_bytes, size_t i_bytecount)
	{
		static_assert(sizeof(size_t) == 8, "This Hash designed for 64bit compile");

		const unsigned char* p = static_cast<const unsigned char*>(i_bytes);

		size_t result = 0xcbf29ce484222325; // FNV offset basis

		while(--i_bytecount)
		{
			result ^= *p++;
			result *= 1099511628211; // FNV prime
		}

		return result;
	};
#else
	constexpr size_t HashedString::Hash(const void* i_bytes, size_t i_bytecount)
	{
		static_assert(sizeof(size_t) == 4, "This Hash designed for 32bit compile");

		const unsigned char* p = static_cast<const unsigned char*>(i_bytes);

		size_t hash = 2166136261;

		for (unsigned int i = 0; i < i_bytecount; ++i)
			hash = 16777619 * (hash ^ p[i]);

		return hash ^ (hash >> 16);
	}
#endif // _X64

}
// namespace Engine
