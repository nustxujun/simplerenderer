#include "ResourceManager.h"
#include "Resource.h"

namespace RCP
{
	ResourceManager::~ResourceManager()
	{
		if (!mResourceList.empty())
			THROW_EXCEPTION("ÓÐ×ÊÔ´Î´ÊÍ");
	}


	bool ResourceManager::find(Resource* res)
	{
		ResourceList::iterator result = 
			mResourceList.find(res);
		return result != mResourceList.end();
	}

	void ResourceManager::destroy(Resource* res)
	{
		ResourceList::iterator result = 
			mResourceList.find(res);
		if (result != mResourceList.end())
		{
			res->subRef();
			if (res->getRefCount()== 0)
			{
				mResourceList.erase(result);
				delete res;
			}
		}

	}

	void ResourceManager::add(Resource* res)
	{
		std::pair<ResourceList::iterator,bool> result = 
			mResourceList.insert(res);
		assert(result.second);
	}

}