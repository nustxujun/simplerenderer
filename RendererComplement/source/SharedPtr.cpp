#include "SharedPtr.h"

namespace RCP
{

	SharedPtr::SharedPtr():
		mRefCount(0)
	{

	}

	SharedPtr::~SharedPtr()
	{
		//��Ȼ�б�ʹ��
		assert( mRefCount == 0);
	}


	void SharedPtr::addRef()
	{
		++mRefCount;
	}

	void SharedPtr::subRef()
	{
		if (mRefCount != 0)
			--mRefCount;
	}

	unsigned int SharedPtr::getRefCount()
	{
		return mRefCount;
	}
}