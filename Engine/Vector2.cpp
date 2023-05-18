#include "Vector2.h"

namespace Engine
{
	const Vector2 Vector2::Zero(0.0f, 0.0f);

	void Vector2::Normalize()
	{
		float length = Length();

		if (!IsZero(length))
		{
			float inv_length = 1.0f / length;

			m_x *= inv_length;
			m_y *= inv_length;
		}
	}

	Vector2 Vector2::Normalized() const
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
