#pragma once

#include "Vector3.h"

namespace Engine
{
	class Matrix4;

	class Matrix3
	{
	public:
		Matrix3();
		Matrix3(
			float i_11, float i_12, float i_13,
			float i_21, float i_22, float i_23,
			float i_31, float i_32, float i_33);
		Matrix3(const Matrix3& i_other);
		Matrix3& operator=(const Matrix3& i_other);

		Matrix3 operator*(const Matrix3& i_other) const;

		Vector2 TransformPointRV(const Vector2& i_Point) const;
		Vector2 TransformPointCV(const Vector2& i_Point) const;

		Vector3 Row(size_t i_row) const;
		Vector3 Col(size_t i_col) const;

		Matrix4 ToTransform() const;

		static Matrix3 CreateIdentity(void);
		static Matrix3 CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ);

		static Matrix3 CreateXRotationRV(float i_RotationRadians);
		static Matrix3 CreateYRotationRV(float i_RotationRadians);
		static Matrix3 CreateZRotationRV(float i_RotationRadians);

		static Matrix3 CreateTranslationRV(float i_transX, float i_transY);
		inline static Matrix3 CreateTranslationRV(const Vector2& i_trans);

		static Matrix3 CreateXRotationCV(float i_RotationRadians);
		static Matrix3 CreateYRotationCV(float i_RotationRadians);
		static Matrix3 CreateZRotationCV(float i_RotationRadians);

		static Matrix3 CreateTranslationCV(float i_transX, float i_transY);
		inline static Matrix3 CreateTranslationCV(const Vector2& i_trans);

		void Transpose(void);
		Matrix3 GetTranspose(void) const;
		void GetTranspose(Matrix3& o_out) const;

		void Multiply(const Matrix3& i_other, Matrix3& o_out) const;

		Vector3 MultiplyLeft(const Vector3& i_Vector) const;

		Vector3 MultiplyRight(const Vector3& i_Vector) const;

		static const Matrix3 Identity;

	private:
		float
			m_11, m_12, m_13, // row 1
			m_21, m_22, m_23, // row 2
			m_31, m_32, m_33; // row 3

		void MultiplyInt(const Matrix3& i_other, Matrix3& o_out) const;

	};

	inline Vector3 operator*(const Matrix3& i_mtx, const Vector3& i_vec);

	inline Vector3 operator*(const Vector3& i_vec, const Matrix3& i_mtx);
} // namespace Engine

#include "Matrix3.inl"
