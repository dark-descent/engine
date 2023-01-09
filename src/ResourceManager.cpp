#include "ResourceManager.hpp"
#include "TextResource.hpp"

namespace DarkDescent
{
	void ResourceManager::onInitialize()
	{
		registerResourceType<TextResource, std::string>();
	}

	void ResourceManager::onReady()
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