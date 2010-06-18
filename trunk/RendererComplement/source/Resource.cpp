#include "Resource.h"
#include "ResourceManager.h"

namespace RCP
{
	Resource::Resource(ResourceManager* mgr):
		mManager(mgr)	
	{
	}

	Resource::~Resource()
	{
	}

	void Resource::Release()
	{
		mManager->destroy(this);
	}

}