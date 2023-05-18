#pragma once

namespace Engine
{
	class Vector3;

	class Vector4
	{
	public:
		Vector4() {}

		inline Vector4(float i_x, float i_y, float i_z, float i_w);

		inline Vector4(const Vector4& i_other);
		inline Vector4(const Vector3& i_other, float i_w);

		// assignment operator
		inline Vector4& operator=(const Vector4& i_rhs);

		inline float x(void) const;
		inline float y(void) const;
		inline float z(void) const;
		inline float w(void) const;

		inline void x(float i_x);
		inline void y(float i_y);
		inline void z(float i_z);
		inline void w(float i_w);

		Vector4& operator+=(const Vector4& i_rhs);
		Vector4& operator-=(const Vector4& i_rhs);

		Vector4& operator*=(float i_val);
		Vector4& operator/=(float i_val);

		Vector4 operator-(void);

		void Normalize();
		Vector4 Normalized() const;

		float Length() const;
		float LengthSq() const;

		static const Vector4 Zero;
	private:
		float m_x, m_y, m_z, m_w;
	};
	inline Vector4 operator+(const Vector4& i_lhs, const Vector4& i_rhs);
	inline Vector4 operator-(const Vector4& i_lhs, const Vector4& i_rhs);

	inline Vector4 operator*(const Vector4& i_lhs, float i_rhs);
	inline Vector4 operator/(const Vector4& i_lhs, float i_rhs);

	inline bool operator==(const Vector4& i_lhs, const Vector4& i_rhs);
	inline bool operator!=(const Vector4& i_lhs, const Vector4& i_rhs);

	inline float dot(const Vector4& i_lhs, const Vector4& i_rhs);
}

#include "Vector4.inl"