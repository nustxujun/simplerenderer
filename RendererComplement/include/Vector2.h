#ifndef _Vector2_H_
#define _Vector2_H_

namespace RCP
{
	class Vector2
	{
   public:
		float x, y;

    public:
		inline Vector2():
			x( 0 ), y( 0 )
        {
        }

        inline Vector2( const float fX, const float fY )
            : x( fX ), y( fY )
        {
        }


        inline Vector2( const Vector2& rkVector )
            : x( rkVector.x ), y( rkVector.y )
        {
        }

        inline Vector2& operator = ( const Vector2& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;

            return *this;
        }


        inline bool operator == ( const Vector2& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y  );
        }

        inline bool operator != ( const Vector2& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y );
        }

        // arithmetic operations
        inline Vector2 operator + ( const Vector2& rkVector ) const
        {
            return Vector2(
                x + rkVector.x,
                y + rkVector.y);
        }

        inline Vector2 operator - ( const Vector2& rkVector ) const
        {
            return Vector2(
                x - rkVector.x,
                y - rkVector.y);
        }

        inline Vector2 operator * ( const float fScalar ) const
        {
            return Vector2(
                x * fScalar,
                y * fScalar);
        }

        inline Vector2 operator * ( const Vector2& rhs) const
        {
            return Vector2(
                x * rhs.x,
                y * rhs.y);
        }

        inline Vector2 operator / ( const float fScalar ) const
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return Vector2(
                x * fInv,
                y * fInv);
        }

        inline Vector2 operator / ( const Vector2& rhs) const
        {
            return Vector2(
                x / rhs.x,
                y / rhs.y);
        }


        // arithmetic updates
        inline Vector2& operator += ( const Vector2& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;

            return *this;
        }


        inline Vector2& operator -= ( const Vector2& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;

            return *this;
        }

        inline Vector2& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            return *this;
        }

        inline Vector2& operator *= ( const Vector2& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;

            return *this;
        }

        inline Vector2& operator /= ( const float fScalar )
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;

            return *this;
        }

        inline Vector2& operator /= ( const Vector2& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;

            return *this;
        }


        inline float length () const
        {
            return sqrt( x * x + y * y  );
        }

        inline float squaredLength () const
        {
            return x * x + y * y  ;
        }

        inline float distance(const Vector2& rhs) const
        {
            return (*this - rhs).length();
        }

        inline float squaredDistance(const Vector2& rhs) const
        {
            return (*this - rhs).squaredLength();
        }

        inline float dotProduct(const Vector2& vec) const
        {
            return x * vec.x + y * vec.y ;
        }

        inline float absDotProduct(const Vector2& vec) const
        {
            return abs(x * vec.x) + abs(y * vec.y);
        }

        inline float normalise()
        {
            float fLength = sqrt( x * x + y * y  );

            // Will also work for zero-sized vectors, but will change nothing
            if ( fLength > 1e-08 )
            {
                float fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;

            }

            return fLength;
        }



		inline Vector2 operator - () const
		{
			return Vector2(-x, -y);
		}

	};
}
#endif//_Vector2_H_
