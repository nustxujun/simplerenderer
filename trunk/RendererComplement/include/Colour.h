#ifndef _Colour_H_
#define _Colour_H_

namespace RCP
{
	/*
		为了方便计算，当颜色值是浮点数时 默认它的范围是[0,1]
		当颜色值是整型时 默认它的范围是 [0,255]
	*/
	class Colour
	{
	public:
		inline Colour():
			r(0),g(0),b(0),a(0)
		{
		}

		inline Colour( float fR, float fG, float fB , float fA = 1.0f)
			: r( fR ), g( fG ), b( fB ), a( fA)
		{
		}

		inline Colour( float f)
			: r( f ), g( f ), b( f ), a( 1.0f )
		{
		}

		inline Colour& operator = ( const Colour& color )
		{
			r = color.r;
			g = color.g;
			b = color.b;
			a = color.a;

			return *this;
		}

		inline Colour operator + ( const Colour& color ) const
		{
			return Colour(
				r + color.r,
				g + color.g,
				b + color.b,
				a + color.a);
		}

		inline Colour operator - ( const Colour& color ) const
		{
			return Colour(
				r - color.r,
				g - color.g,
				b - color.b,
				a - color.a);
		}
		inline Colour operator * ( const Colour& rhs) const
		{
			return Colour(
				rhs.r * r,
				rhs.g * g,
				rhs.b * b,
				rhs.a * a);
		}

		inline Colour operator * ( const float fScalar ) const
        {
            return Colour(
                r * fScalar,
                g * fScalar,
                b * fScalar,
                a * fScalar);
        }

        inline Colour operator / ( const float fScalar ) const
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return Colour(
                r * fInv,
                g * fInv,
                b * fInv,
                a * fInv);
        }

        inline Colour& operator /= ( const float fScalar )
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            r *= fInv;
            g *= fInv;
            b *= fInv;
            a *= fInv;

            return *this;
        }

		inline unsigned int get32BitARGB()const
		{
			unsigned int color;
			color = (((unsigned char)(a*255)) << 24) +
				(((unsigned char)(r*255)) << 16) +
				(((unsigned char)(g*255)) << 8) +
				((unsigned char)(b*255)) ;
			return color;

		}

		inline unsigned int get32BitBGRA()const
		{
			unsigned int color;
			color = (((unsigned char)(b*255)) << 24) +
				(((unsigned char)(g*255)) << 16) +
				(((unsigned char)(r*255)) << 8) +
				((unsigned char)(a*255)) ;
			return color;

		}

		inline unsigned int get32BitRGBA()const
		{
			unsigned int color;
			color = (((unsigned char)(r*255)) << 24) +
				(((unsigned char)(g*255)) << 16) +
				(((unsigned char)(b*255)) << 8) +
				((unsigned char)(a*255)) ;
			return color;

		}

		inline unsigned int get32BitABGR()const
		{
			unsigned int color;
			color = (((unsigned char)(a*255)) << 24) +
				(((unsigned char)(b*255)) << 16) +
				(((unsigned char)(g*255)) << 8) +
				((unsigned char)(r*255)) ;
			return color;

		}

		inline unsigned char get16bitR()const
		{
			return (unsigned char)(r*255);
		}

		inline unsigned char get16bitG()const
		{
			return (unsigned char)(g*255);
		}

		inline unsigned char get16bitB()const
		{
			return (unsigned char)(b*255);
		}

		inline unsigned char get16bitA()const
		{
			return (unsigned char)(a*255);
		}

		inline void getFromARGB(unsigned int argb)
		{
			a = (argb >> 24) / 255.f;
			r = ((argb >> 16) & 0xff) / 255.f;
			g = ((argb >> 8) & 0xff) / 255.f;
			b = (argb & 0xff) / 255.f;
		}

		inline void getFromRGBA(unsigned int rgba)
		{
			r = (rgba >> 24) / 255.f;
			g = ((rgba >> 16) & 0xff) / 255.f;
			b = ((rgba >> 8) & 0xff) / 255.f;
			a = (rgba & 0xff) / 255.f;
		}

		inline void getFromBGRA(unsigned int bgra)
		{
			b = (bgra >> 24) / 255.f;
			g = ((bgra >> 16) & 0xff) / 255.f;
			r = ((bgra >> 8) & 0xff) / 255.f;
			a = (bgra & 0xff) / 255.f;
		}

		inline void getFromABGR(unsigned int abgr)
		{
			a = (abgr >> 24) / 255.f;
			b = ((abgr >> 16) & 0xff) / 255.f;
			g = ((abgr >> 8) & 0xff) / 255.f;
			r = (abgr & 0xff) / 255.f;
		}




		float r,g,b,a;
	};
}
#endif//_Colour_H_
