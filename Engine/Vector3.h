#pragma once

namespace Engine 
{
	class Vector2;

	class Vector3
	{
	public:
		Vector3() { }
		
		inline Vector3(float i_x, float i_y, float i_z);
		inline Vector3(float i_vec[3]);

		inline Vector3(const Vector3& i_rhs);

		inline Vector3(const Vector2& i_other, float i_z);

		inline Vector3& operator=(const Vector3& i_rhs);

		inline float x(void) const;
		inline float y(void) const;
		inline float z(void) const;

		inline void x(float i_x);
		inline void y(float i_y);
		inline void z(float i_z);

		Vector3& operator+=(const Vector3& i_rhs);
		Vector3& operator-=(const Vector3& i_rhs);

		Vector3& operator*=(float i_val);
		Vector3& operator/=(float i_val);

		Vector3 operator-(void);

		void Normalize();
		Vector3 Normalized() const;

		float Length() const;
		float LengthSq() const;

		static const Vector3 Zero;
	private:
		float	m_x, m_y, m_z;
	};

	inline Vector3 operator+(const Vector3& i_lhs, const Vector3& i_rhs);
	inline Vector3 operator-(const Vector3& i_lhs, const Vector3& i_rhs);

	inline Vector3 operator*(const Vector3& i_lhs, const Vector3& i_rhs);
	inline Vector3 operator*(const Vector3& i_lhs, float i_rhs);
	inline Vector3 operator*(float i_lhs, const Vector3& i_rhs);
	inline Vector3 operator/(const Vector3& i_lhs, float i_rhs);

	inline bool operator==(const Vector3& i_lhs, const Vector3& i_rhs);
	inline bool operator!=(const Vector3& i_lhs, const Vector3& i_rhs);

	inline float dot(const Vector3& i_lhs, const Vector3& i_rhs);
	inline Vector3 cross(const Vector3& i_lhs, const Vector3& i_rhs);

} // namespace Engine

#include "Vector3.inl"

