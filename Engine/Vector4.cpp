#include "Vector4.h"
#include "Utilities.h"

#include <math.h>
#include <assert.h>

namespace Engine
{
	const Vector4 Vector4::Zero(0.0f, 0.0f, 0.0f, 0.0f);

	float Vector4::Length() const
	{
		return sqrtf((m_x * m_x) + (m_y * m_y) + (m_z * m_z) + (m_w * m_w));
	}

	Vector4 Vector4::Normalized() const
	{
		float length = Length();
		assert(!IsZero(length));

		if (IsZero(length))
			return Zero;
		else
		{
			float inv_length = 1.0f / length;

			return *this * inv_length;
		}
	}
}