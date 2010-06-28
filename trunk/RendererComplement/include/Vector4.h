#ifndef _Vector4_H_
#define _Vector4_H_
#include"Prerequisites.h"
namespace RCP
{
	class Vector4
	{
	public:
		inline Vector4():
			x(0),y(0),z(0),w(0)
		{
		}

		inline Vector4( float fX, float fY, float fZ = 0, float fW = 0)
			: x( fX ), y( fY ), z( fZ ), w( fW)
		{
		}

		inline Vector4( float f)
			: x( f ), y( f ), z( f ), w( f )
		{
		}

		inline Vector4& operator = ( const Vector4& rkVector )
		{
			x = rkVector.x;
			y = rkVector.y;
			z = rkVector.z;
			w = rkVector.w;

			return *this;
		}

		inline Vector4& operator = ( const Vector3& rkVector )
		{
			x = rkVector.x;
			y = rkVector.y;
			z = rkVector.z;
			w = 0.f;

			return *this;
		}

		inline bool operator == ( const Vector4& rkVector ) const
		{
			return ( x == rkVector.x &&
				y == rkVector.y &&
				z == rkVector.z &&
				w == rkVector.w );
		}

		inline bool operator != ( const Vector4& rkVector ) const
		{
			return ( x != rkVector.x ||
				y != rkVector.y ||
				z != rkVector.z ||
				w != rkVector.w );
		}
		inline Vector4 operator + ( const Vector4& rkVector ) const
		{
			return Vector4(
				x + rkVector.x,
				y + rkVector.y,
				z + rkVector.z,
				w + rkVector.w);
		}

		inline Vector4 operator += ( const Vector4& rkVector )
		{

			x += rkVector.x;
			y += rkVector.y;
			z += rkVector.z;
			w += rkVector.w;
			return *this;
		}

		inline Vector4 operator - ( const Vector4& rkVector ) const
		{
			return Vector4(
				x - rkVector.x,
				y - rkVector.y,
				z - rkVector.z,
				w - rkVector.w);
		}
		inline Vector4 operator * ( const Vector4& rhs) const
		{
			return Vector4(
				rhs.x * x,
				rhs.y * y,
				rhs.z * z,
				rhs.w * w);
		}

		inline Vector4 operator * ( const float fScalar ) const
        {
            return Vector4(
                x * fScalar,
                y * fScalar,
                z * fScalar,
                w * fScalar);
        }

        inline Vector4 operator / ( const float fScalar ) const
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return Vector4(
                x * fInv,
                y * fInv,
                z * fInv,
                w * fInv);
        }

        inline Vector4& operator /= ( const float fScalar )
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;
            w *= fInv;

            return *this;
        }

		inline Vector4& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            w *= fScalar;

            return *this;
        }

		inline float dotProduct(const Vector4& vec) const
		{
			return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
		}

		float x,y,z,w;
	};

	typedef Vector4 Vector2;
}
#endif//_Vector4_H_
