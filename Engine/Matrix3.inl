#include "Utilities.h"

namespace Engine
{
	inline Vector3 operator*(const Matrix3& i_mtx, const Vector3& i_vec)
	{
		return i_mtx.MultiplyRight(i_vec);
	}

	inline Vector3 operator*(const Vector3& i_vec, const Matrix3& i_mtx)
	{
		return i_mtx.MultiplyLeft(i_vec);
	}

	inline Vector2 Matrix3::TransformPointRV(const Vector2& i_point) const
	{
		Vector3 Point = MultiplyLeft(Vector3(i_point, 1.0f));
		assert(!IsZero(Point.z()));

		float inv_z = 1.0f / Point.z();
		return Vector2(Point.x() * inv_z, Point.y() * inv_z);
	}

	inline Vector2 Matrix3::TransformPointCV(const Vector2& i_point) const
	{
		Vector3 Point = MultiplyRight(Vector3(i_point, 1.0f));
		assert(!IsZero(Point.z()));

		float inv_z = 1.0f / Point.z();
		return Vector2(Point.x() * inv_z, Point.y() * inv_z);
	}

	inline Matrix3 Matrix3::operator*(const Matrix3& i_other) const
	{
		assert(!IsNAN(m_11));

		Matrix3 tResult;
		Multiply(i_other, tResult);

		return Matrix3(tResult);
	}

	inline Matrix3 Matrix3::CreateTranslationCV(const Vector2& i_trans)
	{
		return CreateTranslationCV(i_trans.x(), i_trans.y());
	}

	inline Matrix3 Matrix3::CreateTranslationRV(const Vector2& i_trans)
	{
		return CreateTranslationRV(i_trans.x(), i_trans.y());
	}

} // namespace Engine