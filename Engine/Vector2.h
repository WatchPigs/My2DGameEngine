#pragma once

namespace Engine
{
	class Vector2
	{
	public:
		Vector2() { }

		inline Vector2(float i_x, float i_y);
		inline Vector2(float i_vec[2]);

		inline Vector2(const Vector2& i_rhs);

		inline Vector2& operator=(const Vector2& i_rhs);

		inline float x(void) const;
		inline float y(void) const;

		inline void x(float i_x);
		inline void y(float i_y);

		Vector2& operator+=(const Vector2& i_rhs);
		Vector2& operator-=(const Vector2& i_rhs);

		Vector2& operator*=(float i_val);
		Vector2& operator/=(float i_val);

		Vector2 operator-(void);

		void Normalize();
		Vector2 Normalized() const;

		float Length() const;
		float LengthSq() const;

		static const Vector2 Zero;
	private:
		float	m_x, m_y;
	};

	inline Vector2 operator+(const Vector2& i_lhs, const Vector2& i_rhs);
	inline Vector2 operator-(const Vector2& i_lhs, const Vector2& i_rhs);

	inline Vector2 operator*(const Vector2& i_lhs, const Vector2& i_rhs);
	inline Vector2 operator*(const Vector2& i_lhs, float i_rhs);
	inline Vector2 operator*(float i_lhs, const Vector2& i_rhs);
	inline Vector2 operator/(const Vector2& i_lhs, float i_rhs);

	inline bool operator==(const Vector2& i_lhs, const Vector2& i_rhs);
	inline bool operator!=(const Vector2& i_lhs, const Vector2& i_rhs);

	inline float dot(const Vector2& i_lhs, const Vector2& i_rhs);

	//Vector2 operator*(float i, Vector2 p);
} //namespace Engine

#include "Vector2.inl"
