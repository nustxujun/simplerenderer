#include "Resource.h"
#include "ResourceManager.h"

namespace RCP
{
	Resource::Resource(ResourceManager* mgr):
		mManager(mgr),mInitialized(false)	
	{
	}

	Resource::~Resource()
	{
	}

	void Resource::Release()
	{
		mManager->destroy(this);
	}

	void Resource::initialize()
	{
		initImpl();
		mInitialized = true;
	}

	bool Resource::isInitialized()const
	{
		return mInitialized;
	}
}