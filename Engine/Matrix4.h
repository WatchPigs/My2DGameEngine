#pragma once

#include "Vector4.h"

namespace Engine
{
	class Matrix4;
	typedef __declspec(align(64)) Matrix4 FastMatrix4;

	class Matrix4
	{
	public:
		Matrix4();
		Matrix4(
			float i_11, float i_12, float i_13, float i_14,
			float i_21, float i_22, float i_23, float i_24,
			float i_31, float i_32, float i_33, float i_34,
			float i_41, float i_42, float i_43, float i_44);

		Matrix4(const Matrix4& i_other);
		Matrix4& operator=(const Matrix4& i_other);

		inline Matrix4 operator*(const Matrix4& i_other) const;

		inline Vector3 TransformPointRV(const Vector3& i_Point) const;
		inline Vector3 TransformPointCV(const Vector3& i_Point) const;

		inline Vector3 TransformNormalRV(const Vector3& i_Normal) const;
		inline Vector3 TransformNormalCV(const Vector3& i_Normal) const;

		Vector4 Row(size_t i_row) const;
		Vector4 Col(size_t i_col) const;

		static Matrix4 CreateIdentity(void);
		static Matrix4 CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ);

		static Matrix4 CreateXRotationRV(float i_RotationRadians);
		static Matrix4 CreateYRotationRV(float i_RotationRadians);
		static Matrix4 CreateZRotationRV(float i_RotationRadians);

		static Matrix4 CreateTranslationRV(float i_transX, float i_transY, float i_transZ);
		inline static Matrix4 CreateTranslationRV(const Vector3& i_trans);

		static Matrix4 CreateXRotationCV(float i_RotationRadians);
		static Matrix4 CreateYRotationCV(float i_RotationRadians);
		static Matrix4 CreateZRotationCV(float i_RotationRadians);

		static Matrix4 CreateTranslationCV(float i_transX, float i_transY, float i_transZ);
		inline static Matrix4 CreateTranslationCV(const Vector3& i_trans);

		inline void Invert(void);

		inline Matrix4 GetInverse(void) const;
		inline void GetInverse(Matrix4& o_out) const;

		Matrix4 GetInverseRotTransCV() const;
		Matrix4 GetInverseRotTransRV() const;

		void Transpose(void);
		Matrix4 GetTranspose(void) const;
		void GetTranspose(Matrix4& o_out) const;

		void Multiply(const Matrix4& i_other, Matrix4& o_out) const;

		Vector4 MultiplyLeft(const Vector4& i_Vector) const;
		// return M * v
		Vector4 MultiplyRight(const Vector4& i_Vector) const;

		static const FastMatrix4 Identity;

	private:
		float
			m_11, m_12, m_13, m_14, 
			m_21, m_22, m_23, m_24, 
			m_31, m_32, m_33, m_34, 
			m_41, m_42, m_43, m_44; 

		void MultiplyInt(const Matrix4& i_other, Matrix4& o_out) const;

		void InverseSSE(Matrix4& o_out) const;
		void MultiplySSE(const Matrix4& i_other, Matrix4& o_out) const;
	};

	inline Vector4 operator*(const Matrix4& i_mtx, const Vector4& i_vec);

	inline Vector4 operator*(const Vector4& i_vec, const Matrix4& i_mtx);
} // namespace Engine

#include "Matrix4.inl"

