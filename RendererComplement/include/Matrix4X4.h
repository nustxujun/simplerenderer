#ifndef _Matrix4X4_H_
#define _Matrix4X4_H_
#include "Vector4.h"
#include "Vector3.h"
namespace RCP
{
	class Matrix4X4
	{
	public:
		const static Matrix4X4 IDENTITY;
	public:
		float m[4][4];
	public :

		inline Matrix4X4(	
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;
			m[0][3] = m03;
			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;
			m[1][3] = m13;
			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
			m[2][3] = m23;
			m[3][0] = m30;
			m[3][1] = m31;
			m[3][2] = m32;
			m[3][3] = m33;

		}

		inline Matrix4X4()
		{
			m[0][0] = 1;
			m[0][1] = 0;
			m[0][2] = 0;
			m[0][3] = 0;
			m[1][0] = 0;
			m[1][1] = 1;
			m[1][2] = 0;
			m[1][3] = 0;
			m[2][0] = 0;
			m[2][1] = 0;
			m[2][2] = 1;
			m[2][3] = 0;
			m[3][0] = 0;
			m[3][1] = 0;
			m[3][2] = 0;
			m[3][3] = 1;

		}


		inline Matrix4X4 operator * (const Matrix4X4& m2)const
		{
			Matrix4X4 r;
			r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
			r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
			r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
			r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

			r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
			r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
			r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
			r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

			r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
			r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
			r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
			r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

			r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
			r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
			r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
			r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

			return r;
		}


		inline void operator = ( const Matrix4X4& mat4 )
		{
			m[0][0] = mat4.m[0][0]; m[0][1] = mat4.m[0][1]; m[0][2] = mat4.m[0][2];m[0][3] = mat4.m[0][3];
			m[1][0] = mat4.m[1][0]; m[1][1] = mat4.m[1][1]; m[1][2] = mat4.m[1][2];m[1][3] = mat4.m[1][3];
			m[2][0] = mat4.m[2][0]; m[2][1] = mat4.m[2][1]; m[2][2] = mat4.m[2][2];m[2][3] = mat4.m[2][3];
			m[3][0] = mat4.m[3][0]; m[3][1] = mat4.m[3][1]; m[3][2] = mat4.m[3][2];m[3][3] = mat4.m[3][3];
		}

		inline Matrix4X4 operator*(float scalar) const
		{
			return Matrix4X4(
				scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
				scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
				scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
				scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
		}

		inline Matrix4X4 operator + ( const Matrix4X4 &m2 ) const
		{
			Matrix4X4 r;

			r.m[0][0] = m[0][0] + m2.m[0][0];
			r.m[0][1] = m[0][1] + m2.m[0][1];
			r.m[0][2] = m[0][2] + m2.m[0][2];
			r.m[0][3] = m[0][3] + m2.m[0][3];

			r.m[1][0] = m[1][0] + m2.m[1][0];
			r.m[1][1] = m[1][1] + m2.m[1][1];
			r.m[1][2] = m[1][2] + m2.m[1][2];
			r.m[1][3] = m[1][3] + m2.m[1][3];

			r.m[2][0] = m[2][0] + m2.m[2][0];
			r.m[2][1] = m[2][1] + m2.m[2][1];
			r.m[2][2] = m[2][2] + m2.m[2][2];
			r.m[2][3] = m[2][3] + m2.m[2][3];

			r.m[3][0] = m[3][0] + m2.m[3][0];
			r.m[3][1] = m[3][1] + m2.m[3][1];
			r.m[3][2] = m[3][2] + m2.m[3][2];
			r.m[3][3] = m[3][3] + m2.m[3][3];

			return r;
		}

		inline Vector4 operator * (const Vector4& v) const
		{
			return Vector4(
				m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w, 
				m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
				m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
				m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
				);
		}

		inline Vector3 operator * ( const Vector3 &v ) const
		{
			Vector3 r;

			float fInvW = 1.0f / ( m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] );

			r.x = ( m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] ) * fInvW;
			r.y = ( m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] ) * fInvW;
			r.z = ( m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] ) * fInvW;

			return r;
		}

		Matrix4X4 Matrix4X4::operator- () const
		{
			Matrix4X4 kNeg;
			for (size_t iRow = 0; iRow < 4; iRow++)
			{
				for (size_t iCol = 0; iCol < 4; iCol++)
					kNeg.m[iRow][iCol] = -m[iRow][iCol];
			}
			return kNeg;
		}

		inline bool isAffine(void) const
        {
            return m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1;
        }

		Matrix4X4 Matrix4X4::inverseAffine(void) const
		{
			assert(isAffine());

			float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2];
			float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2];

			float t00 = m22 * m11 - m21 * m12;
			float t10 = m20 * m12 - m22 * m10;
			float t20 = m21 * m10 - m20 * m11;

			float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2];

			float invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

			t00 *= invDet; t10 *= invDet; t20 *= invDet;

			m00 *= invDet; m01 *= invDet; m02 *= invDet;

			float r00 = t00;
			float r01 = m02 * m21 - m01 * m22;
			float r02 = m01 * m12 - m02 * m11;

			float r10 = t10;
			float r11 = m00 * m22 - m02 * m20;
			float r12 = m02 * m10 - m00 * m12;

			float r20 = t20;
			float r21 = m01 * m20 - m00 * m21;
			float r22 = m00 * m11 - m01 * m10;

			float m03 = m[0][3], m13 = m[1][3], m23 = m[2][3];

			float r03 = - (r00 * m03 + r01 * m13 + r02 * m23);
			float r13 = - (r10 * m03 + r11 * m13 + r12 * m23);
			float r23 = - (r20 * m03 + r21 * m13 + r22 * m23);

			return Matrix4X4(
				r00, r01, r02, r03,
				r10, r11, r12, r13,
				r20, r21, r22, r23,
				0,   0,   0,   1);
		}

	};


}
#endif//_Matrix4X4_H_
