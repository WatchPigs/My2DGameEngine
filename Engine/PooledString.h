#pragma once

#include "StringPool.h"

namespace Engine
{
	class PooledString
	{
	public:
		explicit PooledString(const char* i_pString) :
			m_pString(GlobalStringPool().find(i_pString))
		{}

		PooledString(const PooledString& i_other) :
			m_pString(i_other.m_pString)
		{}

		PooledString& operator=(const PooledString& i_other)
		{
			m_pString = i_other.m_pString;
		}

		bool operator==(const PooledString& i_other) const
		{
			return m_pString == i_other.m_pString;
		}

		bool operator==(const char* i_pString) const
		{
			return m_pString == GlobalStringPool().find(i_pString);
		}

		bool operator<(const PooledString& i_other) const
		{
			return m_pString < i_other.m_pString;
		}

		operator bool() const
		{
			return m_pString != nullptr;
		}

		operator const char* () const
		{
			return m_pString;
		}

		struct less
		{
		public:
			bool operator()(const PooledString& i_lhs, const PooledString& i_rhs) const
			{
				return i_lhs < i_rhs;
			}
		};
	private:
		const char* m_pString;
	};

}

