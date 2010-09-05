#ifndef _Math_H_
#define _Math_H_
#include <Math.h>
namespace RCP
{
	inline float log2(float f)
	{
		return log(f) * 1.442695f;
	}

	template<class T>
	inline T clamp(const T& value, const T& min, const T& max)
	{
		assert(min <= max);
		if (value < min )
			return min;
		if (value > max)
			return max;

		return value;
	}

	template <class T>
	inline void lerp(T& output, float t,const T& v1, const T& v2)
	{
		output = v1 + (v2 - v1) * t;
	}

	template <class T>
	bool equal(const T& in1, const T& in2)
	{
		if((T)(std::abs(in1 - in2)) <= std::numeric_limits<T>::epsilon()) return true;
		return false;
	}

	inline unsigned int fastCeil(float val)
	{
		float f = floor(val);
		//0.01f的精度控制。避免过小小数部分而导致进位。现象是，会出现奇怪的底色杂点
		if (val > f + 0.01f)
			return (unsigned int)f + 1;
		else 
			return (unsigned int)f;
	}

	inline unsigned int fastFloor(float val)
	{
		return (unsigned int )val;
	}
}
#endif//_Math_H_
