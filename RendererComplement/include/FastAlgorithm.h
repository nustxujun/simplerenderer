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
		return (unsigned int)ceil(val);
	}

	inline unsigned int floor(float val)
	{
		return (unsigned int )val;
	}
}
#endif//_Math_H_
