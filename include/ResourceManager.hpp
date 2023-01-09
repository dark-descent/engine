#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"
#include "Hash.hpp"
#include "Resource.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	class ResourceManager: public SubSystem
	{
		SUB_SYSTEM_CTORS(ResourceManager) { };

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;

	public:
		template<IsResource T, typename Data>
		void registerResourceType()
		{
			assert(!T::isRegistered());
			Resource<T, Data>::index_ = resourceMaps_.size();
			Resource<T, Data>::isRegistered_ = true;
			resourceMaps_.emplace_back();
		}

		template<IsResource T>
		void registerResource(const char* path)
		{
			const Hash hash = Hasher::hash(path);
			std::unordered_map<Hash, IResource*>& map = resourceMaps_.at(T::index());
			if (!map.contains(hash))
			{
				map.emplace(hash, new T(path));
			}
		}

		template<IsResource T>
		void registerResource(const std::string& path)
		{
			registerResource(path.c_str());
		}

		template<IsResource T>
		T& loadResource(const char* path)
		{
			const Hash hash = Hasher::hash(path);
			std::unordered_map<Hash, IResource*>& map = resourceMaps_.at(T::index());

			if (!map.contains(hash))
			{
				auto resource = new T(path);
				map.emplace(hash, resource);
				pendingResources_.emplace_back(resource);
				return *resource;
			}
			else
			{
				T& resource = getResource<T>(path);
				pendingResources_.emplace_back(std::addressof(resource));
				return resource;
			}
		}

		template<IsResource T>
		T& loadResource(const std::string& path)
		{
			return loadResource<T>(path.c_str());
		}

		template<IsResource T>
		T& loadResourceImmediate(const char* path)
		{
			Hash hash = Hasher::hash(path);

			std::unordered_map<Hash, IResource*>& map = resourceMaps_.at(T::index());

			if (!map.contains(hash))
				registerResource<T>(path);

			auto& r = getResource<T>(path);
			r.load();
			return r;
		}

		template<IsResource T>
		T& loadResourceImmediate(const std::string& path)
		{
			return loadResourceImmediate<T>(path.c_str());
		}

		template<IsResource T>
		T& getResource(const std::string& path)
		{
			Hash hash = Hasher::hash(path.c_str());

			std::unordered_map<Hash, IResource*>& map = resourceMaps_.at(T::index());

			if (!map.contains(hash))
				throw TraceException("Could not find resource! (maybe not registered?)");

			return *static_cast<T*>(map.at(hash));
		}

		void loadResources();

	private:
		std::vector<IResource*> pendingResources_;
		std::vector<std::unordered_map<Hash, IResource*>> resourceMaps_;
	};
}