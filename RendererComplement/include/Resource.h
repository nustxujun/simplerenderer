#ifndef _Resource_H_
#define _Resource_H_
#include"Prerequisites.h"
#include "SharedPtr.h"

namespace RCP
{
	class Resource:
		public SharedPtr
	{
	public :
		Resource(ResourceManager* mgr);
		virtual ~Resource();
		void release();
		void initialize();
		virtual void initImpl() = 0;
		bool isInitialized()const;
	private:
		ResourceManager* mManager;
	private:
		//²»ÓÃµÄ
		Resource();
		Resource(const Resource& res);
		void operator = (const Resource& res);
	private:
		bool mInitialized;
	};
}
#endif//_Resource_H_
