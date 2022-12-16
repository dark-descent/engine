#include "ResourceManager.hpp"

namespace DarkDescent
{
	void ResourceManager::onInitialize()
	{
		
	}
			
	void ResourceManager::onTerminate()
	{

	}

	void ResourceManager::loadResources()
	{
		for(auto r : pendingResources_)
			r->load();
		pendingResources_.clear();
	}
}