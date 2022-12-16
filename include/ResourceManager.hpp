#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"
#include "Hash.hpp"
#include "Resource.hpp"

namespace DarkDescent
{
	class ResourceManager: public SubSystem
	{
		SUB_SYSTEM_CTORS(ResourceManager);

	protected:
		virtual void onInitialize() override;
		virtual void onTerminate() override;

	public:
		template<typename T>
		const T& getResource(const char* path)
		{
			Hash hash = Hasher::hash(path);
			if (!resources_.contains(hash))
				resources_.emplace(hash, T(path));
			T& resource = resources_.at(hash);
			pendingResources_.emplace_back(resource);
			return resource;
		}

		void loadResources();

	private:
		std::vector<Resource*> pendingResources_;
		std::unordered_map<Hash, Resource> resources_;
	};
}