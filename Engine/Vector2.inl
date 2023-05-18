#include <assert.h>

#include "Utilities.h"

namespace Engine {
	inline Vector2::Vector2(float i_x, float i_y) :
		m_x(i_x),
		m_y(i_y)
	{
		assert(!IsNAN(i_x));
		assert(!IsNAN(i_y));
	}

	inline Vector2::Vector2(float i_vec[2]) :
		m_x(i_vec[0]),
		m_y(i_vec[1])
	{
		assert(!IsNAN(i_vec[0]));
		assert(!IsNAN(i_vec[1]));
	}

	inline Vector2::Vector2(const Vector2& i_rhs) :
		m_x(i_rhs.m_x),
		m_y(i_rhs.m_y)
	{
		assert(!IsNAN(i_rhs.m_x));
		assert(!IsNAN(i_rhs.m_y));
	}

	inline Vector2& Vector2::operator=(const Vector2& i_rhs)
	{
		assert(!IsNAN(i_rhs.m_x));
		assert(!IsNAN(i_rhs.m_y));

		m_x = i_rhs.m_x;
		m_y = i_rhs.m_y;

		return *this;
	}

	inline float Vector2::x(void) const
	{
		return m_x;
	}

	inline float Vector2::y(void) const
	{
		return m_y;
	}

	inline void Vector2::x(float i_x)
	{
		assert(!IsNAN(i_x));

		m_x = i_x;
	}

	inline void Vector2::y(float i_y)
	{
		assert(!IsNAN(i_y));

		m_y = i_y;
	}

	inline Vector2& Vector2::operator+=(const Vector2& i_rhs)
	{
		m_x += i_rhs.m_x;
		m_y += i_rhs.m_y;

		return *this;
	}

	inline Vector2& Vector2::operator-=(const Vector2& i_rhs)
	{
		m_x -= i_rhs.m_x;
		m_y -= i_rhs.m_y;

		return *this;
	}

	inline Vector2& Vector2::operator*=(float i_rhs)
	{
		assert(!IsNAN(i_rhs));

		m_x *= i_rhs;
		m_y *= i_rhs;

		return *this;
	}

	inline Vector2& Vector2::operator/=(float i_rhs)
	{
		assert(!IsNAN(i_rhs));
		assert(!IsZero(i_rhs));

		m_x /= i_rhs;
		m_y /= i_rhs;

		return *this;
	}

	inline Vector2 Vector2::operator-(void)
	{
		return Vector2(-m_x, -m_y);
	}

	inline float Vector2::LengthSq() const
	{
		return (m_x * m_x) + (m_y * m_y);
	}

	inline float Vector2::Length() const
	{
		return static_cast<float>(sqrt(LengthSq()));
	}

	inline Vector2 operator+(const Vector2& i_lhs, const Vector2& i_rhs)
	{
		return Vector2(i_lhs.x() + i_rhs.x(), i_lhs.y() + i_rhs.y());
	}

	inline Vector2 operator-(const Vector2& i_lhs, const Vector2& i_rhs)
	{
		return Vector2(i_lhs.x() - i_rhs.x(), i_lhs.y() - i_rhs.y());
	}

	inline Vector2 operator*(const Vector2& i_lhs, const Vector2& i_rhs)
	{

		return Vector2(i_lhs.x() * i_rhs.x(), i_lhs.y() * i_rhs.y());
	}

	inline Vector2 operator*(const Vector2& i_lhs, float i_rhs)
	{
		assert(!IsNAN(i_rhs));

		return Vector2(i_lhs.x() * i_rhs, i_lhs.y() * i_rhs);
	}

	inline Vector2 operator*(float i_lhs, const Vector2& i_rhs)
	{
		assert(!IsNAN(i_lhs));

		return Vector2(i_rhs.x() * i_lhs, i_rhs.y() * i_lhs);
	}

	inline Vector2 operator/(const Vector2& i_lhs, float i_rhs)
	{
		assert(!IsNAN(i_rhs));
		assert(!IsZero(i_rhs));

		return Vector2(i_lhs.x() / i_rhs, i_lhs.y() / i_rhs);
	}

	inline bool operator==(const Vector2& i_lhs, const Vector2& i_rhs)
	{
		return AreEqual_Eps(i_lhs.x(), i_rhs.x()) && AreEqual_Eps(i_lhs.y(), i_rhs.y());
	}

	inline bool operator!=(const Vector2& i_lhs, const Vector2& i_rhs)
	{
		return !AreEqual_Eps(i_lhs.x(), i_rhs.x()) || !AreEqual_Eps(i_lhs.y(), i_rhs.y());
	}

	inline float dot(const Vector2& i_lhs, const Vector2& i_rhs)
	{
		return i_lhs.x() * i_rhs.x() + i_lhs.y() * i_rhs.y();
	}
}