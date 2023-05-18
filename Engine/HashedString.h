#pragma once

namespace Engine
{
	class HashedString
	{
	public:
		constexpr HashedString();
		constexpr HashedString(const char* i_string);

		HashedString(const HashedString& i_other);
		HashedString& operator=(const HashedString& i_other);

		~HashedString();
		size_t get() const;

		bool operator==(const HashedString& i_other) const;
		bool operator<(const HashedString& i_other) const { return m_Hash < i_other.m_Hash; }

	private:
		static constexpr size_t Hash(const char* i_string);
		static constexpr size_t Hash(const void* i_bytes, size_t i_count);

		size_t	m_Hash;

#ifdef DEBUG_KEEP_STRING
		const char* m_pString;
#endif
	};

} // namespace Engine

#include "HashedString.inl"