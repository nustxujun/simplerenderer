#ifndef _ResourceManager_H_
#define _ResourceManager_H_
#include"Prerequisites.h"

namespace RCP
{
	class ResourceManager
	{
	public:
		virtual ~ResourceManager();

		bool find(Resource* res);
		void destroy(Resource* res);
	protected:
		void add(Resource* res);
		
	private:
		typedef std::set<Resource*> ResourceList;
		ResourceList mResourceList;

	};
}
#endif//_ResourceManager_H_
