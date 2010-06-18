#ifndef _SharedPtr_H_
#define _SharedPtr_H_

#include "Prerequisites.h"

namespace RCP
{
	//��ӛ�������]�e����
	class SharedPtr
	{
	public:
		SharedPtr();
		virtual ~SharedPtr();

		void addRef();
		void subRef();
		unsigned int getRefCount();
	private:
		unsigned int mRefCount;
	};
}
#endif//_SharedPtr_H_
