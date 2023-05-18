namespace Engine
{
	inline Vector4 operator*(const Matrix4& i_mtx, const Vector4& i_vec)
	{
		return i_mtx.MultiplyRight(i_vec);
	}

	inline Vector4 operator*(const Vector4& i_vec, const Matrix4& i_mtx)
	{
		return i_mtx.MultiplyLeft(i_vec);
	}

	inline Vector3 Matrix4::TransformPointCV(const Vector3& i_point) const
	{
		Vector4 Point = MultiplyRight(Vector4(i_point, 1.0f));
		assert(!IsZero(Point.w()));

		float inv_w = 1.0f / Point.w();
		return Vector3(Point.x() * inv_w, Point.y() * inv_w, Point.z() * inv_w);
	}

	inline Vector3 Matrix4::TransformPointRV(const Vector3& i_point) const
	{
		Vector4 Point = MultiplyLeft(Vector4(i_point, 1.0f));
		assert(!IsZero(Point.w()));

		float inv_w = 1.0f / Point.w();
		return Vector3(Point.x() * inv_w, Point.y() * inv_w, Point.z() * inv_w);
	}

	inline Vector3 Matrix4::TransformNormalCV(const Vector3& i_point) const
	{
		Vector4 Point = MultiplyRight(Vector4(i_point, 0.0f));
		return Vector3(Point.x(), Point.y(), Point.z());
	}

	inline Vector3 Matrix4::TransformNormalRV(const Vector3& i_point) const
	{
		Vector4 Point = MultiplyLeft(Vector4(i_point, 0.0f));
		return Vector3(Point.x(), Point.y(), Point.z());
	}

	inline void Matrix4::Invert(void)
	{
		assert(!IsNAN(m_11));

		InverseSSE(*this);
	}

	inline Matrix4 Matrix4::GetInverse(void) const
	{
		assert(!IsNAN(m_11));

		Matrix4	Temporary;

		InverseSSE(Temporary);

		return Matrix4(Temporary);
	}

	inline void Matrix4::GetInverse(Matrix4& o_out) const
	{
		InverseSSE(o_out);
	}

	inline Matrix4 Matrix4::operator*(const Matrix4& i_other) const
	{
		assert(!IsNAN(m_11));

#if defined(_WIN32)
		if ((reinterpret_cast<uintptr_t>(&i_other) & (16 - 1)) == 0)
		{
			FastMatrix4 tResult;
			MultiplySSE(i_other, tResult);

			return Matrix4(tResult);
		}
#endif

		Matrix4 tResult;
		Multiply(i_other, tResult);

		return Matrix4(tResult);
	}

	inline Matrix4 Matrix4::CreateTranslationCV(const Vector3& i_trans)
	{
		return CreateTranslationCV(i_trans.x(), i_trans.y(), i_trans.z());
	}

	inline Matrix4 Matrix4::CreateTranslationRV(const Vector3& i_trans)
	{
		return CreateTranslationRV(i_trans.x(), i_trans.y(), i_trans.z());
	}
} // namespace Engine