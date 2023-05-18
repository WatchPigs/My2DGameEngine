#include "Vector3.h"

namespace Engine {
	const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);

	void Vector3::Normalize()
	{
		float length = Length();

		if (!IsZero(length))
		{
			float inv_length = 1.0f / length;

			m_x *= inv_length;
			m_y *= inv_length;
			m_z *= inv_length;
		}
	}

	Vector3 Vector3::Normalized() const
	{
		float length = Length();

		if (IsZero(length))
			return Zero;
		else
		{
			float inv_length = 1.0f / length;

			return *this * inv_length;
		}
	}
}