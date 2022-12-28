#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"
#include "Hash.hpp"
#include "Resource.hpp"

namespace DarkDescent
{
	class ResourceManager: public SubSystem
	{
		SUB_SYSTEM_CTORS(ResourceManager) {};

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;

	public:
		const Resource& getResource(const char* path)
		{
			Hash hash = Hasher::hash(path);
			if (!resources_.contains(hash))
				resources_.emplace(hash, Resource(path));
			Resource* resource = &resources_.at(hash);
			pendingResources_.emplace_back(resource);
			return *resource;
		}

		void loadResources();

	private:
		std::vector<Resource*> pendingResources_;
		std::unordered_map<Hash, Resource> resources_;
	};
}