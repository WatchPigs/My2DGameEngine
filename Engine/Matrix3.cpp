#include "Matrix3.h"

#include "Matrix4.h"

#include <assert.h>
#include <math.h>
#include <stdint.h>

namespace Engine
{
	const Matrix3 Matrix3::Identity(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f);

	Matrix3::Matrix3(void)
	{
#ifdef _DEBUG
		// In a _DEBUG build we use this to signal that we're not initialized in case we
		// call a using method (Transpose(), Invert(), operator*(), etc.) before setting the
		// matrix via one of the Create<xxxxx>() methods.
		m_11 = NAN;
#endif
	}

	Matrix3::Matrix3(
		float i_11, float i_12, float i_13,
		float i_21, float i_22, float i_23,
		float i_31, float i_32, float i_33)
		:
		m_11(i_11), m_12(i_12), m_13(i_13),
		m_21(i_21), m_22(i_22), m_23(i_23),
		m_31(i_31), m_32(i_32), m_33(i_33)
	{

	}

	Matrix3::Matrix3(const Matrix3& i_other)
		:
		m_11(i_other.m_11), m_12(i_other.m_12), m_13(i_other.m_13),
		m_21(i_other.m_21), m_22(i_other.m_22), m_23(i_other.m_23),
		m_31(i_other.m_31), m_32(i_other.m_32), m_33(i_other.m_33)
	{

	}

	Matrix3& Matrix3::operator=(const Matrix3& i_other)
	{
		assert(!IsNAN(i_other.m_11));

		m_11 = i_other.m_11; m_12 = i_other.m_12; m_13 = i_other.m_13;
		m_21 = i_other.m_21; m_22 = i_other.m_22; m_23 = i_other.m_23;
		m_31 = i_other.m_31; m_32 = i_other.m_32; m_33 = i_other.m_33;

		return *this;
	}

	Matrix4 Matrix3::ToTransform() const
	{
		assert(!IsNAN(m_11));

		return Matrix4(
			m_11, m_12, m_13, 0.0f,
			m_21, m_22, m_23, 0.0f,
			m_31, m_32, m_33, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Matrix3::Transpose(void)
	{
		assert(!IsNAN(m_11));

		register float temp;

		// swap row 1 and column 1 except m_11
		temp = m_21; m_21 = m_12; m_12 = temp;
		temp = m_31; m_31 = m_13; m_13 = temp;

		temp = m_32; m_32 = m_23; m_23 = temp;
	}

	Matrix3 Matrix3::GetTranspose(void) const
	{
		assert(!IsNAN(m_11));

		return Matrix3(
			m_11, m_21, m_31,
			m_12, m_22, m_32,
			m_13, m_23, m_33);
	}

	void Matrix3::GetTranspose(Matrix3& o_out) const
	{
		assert(!IsNAN(m_11));

		o_out.m_11 = m_11; o_out.m_12 = m_21; o_out.m_13 = m_31;
		o_out.m_21 = m_12; o_out.m_22 = m_22; o_out.m_23 = m_32;
		o_out.m_31 = m_13; o_out.m_32 = m_23; o_out.m_33 = m_33;
	}

	Vector3 Matrix3::MultiplyLeft(const Vector3& i_Vector) const
	{
		assert(!IsNAN(m_11));

		return Vector3(
			i_Vector.x() * m_11 + i_Vector.y() * m_21 + i_Vector.z() * m_31,
			i_Vector.x() * m_12 + i_Vector.y() * m_22 + i_Vector.z() * m_32,
			i_Vector.x() * m_13 + i_Vector.y() * m_23 + i_Vector.z() * m_33);
	}

	Vector3 Matrix3::MultiplyRight(const Vector3& i_Vector) const
	{
		assert(!IsNAN(m_11));

		return Vector3(
			i_Vector.x() * m_11 + i_Vector.y() * m_12 + i_Vector.z() * m_13,
			i_Vector.x() * m_21 + i_Vector.y() * m_22 + i_Vector.z() * m_23,
			i_Vector.x() * m_31 + i_Vector.y() * m_32 + i_Vector.z() * m_33);
	}

	void Matrix3::MultiplyInt(const Matrix3& i_other, Matrix3& o_out) const
	{
		assert(!IsNAN(m_11));
		assert(!IsNAN(i_other.m_11));

		o_out.m_11 = m_11 * i_other.m_11 + m_12 * i_other.m_21 + m_13 * i_other.m_31;
		o_out.m_12 = m_11 * i_other.m_12 + m_12 * i_other.m_22 + m_13 * i_other.m_32;
		o_out.m_13 = m_11 * i_other.m_13 + m_12 * i_other.m_23 + m_13 * i_other.m_33;

		o_out.m_21 = m_21 * i_other.m_11 + m_22 * i_other.m_21 + m_23 * i_other.m_31;
		o_out.m_22 = m_21 * i_other.m_12 + m_22 * i_other.m_22 + m_23 * i_other.m_32;
		o_out.m_23 = m_21 * i_other.m_13 + m_22 * i_other.m_23 + m_23 * i_other.m_33;

		o_out.m_31 = m_31 * i_other.m_11 + m_32 * i_other.m_21 + m_33 * i_other.m_31;
		o_out.m_32 = m_31 * i_other.m_12 + m_32 * i_other.m_22 + m_33 * i_other.m_32;
		o_out.m_33 = m_31 * i_other.m_13 + m_32 * i_other.m_23 + m_33 * i_other.m_33;
	}

	void Matrix3::Multiply(const Matrix3& i_other, Matrix3& o_out) const
	{
		assert(!IsNAN(m_11));
		assert(!IsNAN(i_other.m_11));

		MultiplyInt(i_other, o_out);
	}


	Matrix3 Matrix3::CreateIdentity(void)
	{
		return Matrix3(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f);
	}

	Matrix3 Matrix3::CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ)
	{
		return Matrix3(
			i_scaleX, 0.0f, 0.0f,
			0.0f, i_scaleY, 0.0f,
			0.0f, 0.0f, i_scaleZ);
	}

	Matrix3 Matrix3::CreateXRotationCV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3(
			1.0f, 0.0f, 0.0f,
			0.0f, cos, -sin,
			0.0f, sin, cos);
	}

	Matrix3 Matrix3::CreateYRotationCV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3(
			cos, 0.0f, sin,
			0.0f, 1.0f, 0.0f,
			-sin, 0.0f, cos);
	}
	Matrix3 Matrix3::CreateZRotationCV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3(
			cos, -sin, 0.0f,
			sin, cos, 0.0f,
			0.0f, 0.0f, 1.0f);
	}

	Matrix3 Matrix3::CreateTranslationCV(float i_transX, float i_transY)
	{
		return Matrix3(
			1.0f, 0.0f, i_transX,
			0.0f, 1.0f, i_transY,
			0.0f, 0.0f, 1.0f);
	}

	Matrix3 Matrix3::CreateXRotationRV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3(
			1.0f, 0.0f, 0.0f,
			0.0f, cos, sin,
			0.0f, -sin, cos);
	}

	Matrix3 Matrix3::CreateYRotationRV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3(
			cos, 0.0f, -sin,
			0.0f, 1.0f, 0.0f,
			sin, 0.0f, cos);
	}
	Matrix3 Matrix3::CreateZRotationRV(float i_RotationRadians)
	{
		float sin = sinf(i_RotationRadians);
		float cos = cosf(i_RotationRadians);

		return Matrix3(
			cos, sin, 0.0f,
			-sin, cos, 0.0f,
			0.0f, 0.0f, 1.0f);
	}

	Matrix3 Matrix3::CreateTranslationRV(float i_transX, float i_transY)
	{
		return Matrix3(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			i_transX, i_transY, 1.0f);
	}

	Vector3 Matrix3::Row(size_t i_row) const
	{
		assert(!IsNAN(m_11));

		switch (i_row)
		{
		case 0:
			return Vector3(m_11, m_12, m_13);
		case 1:
			return Vector3(m_21, m_22, m_23);
		case 2:
			return Vector3(m_31, m_32, m_33);
		default:
			assert(false);
			return Vector3(NAN, NAN, NAN);
		}
	}

	Vector3 Matrix3::Col(size_t i_col) const
	{
		assert(!IsNAN(m_11));

		switch (i_col)
		{
		case 0:
			return Vector3(m_11, m_21, m_31);
		case 1:
			return Vector3(m_12, m_22, m_32);
		case 2:
			return Vector3(m_13, m_23, m_33);
		default:
			assert(false);
			return Vector3(NAN, NAN, NAN);
		}
	}
} // namespace Engine