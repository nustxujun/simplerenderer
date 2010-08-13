#ifndef _Array_H_
#define _Array_H_

namespace RCP
{
	template<class T,unsigned int size>
	class Array
	{
		T mElems[size];
		bool mIsUsed[size];
	public :
		Array()
		{
			memset(mIsUsed,0,sizeof(bool) * size);
		}

		T& operator [](unsigned int index)
		{
			assert(index < size);
			mIsUsed[index] = true;
			return mElems[index];
		}

		const T& operator [](unsigned int index)const 
		{
			assert(index < size);
			return mElems[index];
		}

		bool isUsed(unsigned int index)const 
		{
			assert(index < size);
			return mIsUsed[index];
		}
	};
}
#endif//_Array_H_
