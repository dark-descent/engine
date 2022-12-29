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

		template<IsResource T, typename... Args>
		void registerResource(std::unordered_map<Hash, IResource*>& map, Hash hash, const char* path, Args&&... args)
		{
			if (!map.contains(hash))
				map.emplace(hash, static_cast<IResource*>(new T(std::forward<Args>(args)...)));
		}

		template<IsResource T, typename... Args>
		void registerResource(Hash hash, const char* path, Args&&... args)
		{
			std::unordered_map<Hash, IResource*>& map = resourceMaps_.at(T::index());
			registerResource<T, Args...>(map, hash, path, std::forward<Args>(args)...);
		}

		template<IsResource T, typename... Args>
		void registerResource(const char* path, Args&&... args)
		{
			registerResource<T, Args...>(Hasher::hash(path), path, std::forward<Args>(args)...);
		}

		template<IsResource T, typename... Args>
		void registerResource(const std::string& path, Args&&... args)
		{
			std::unordered_map<Hash, IResource*>& map = resourceMaps_.at(T::index());
			registerResource<T, Args...>(map, Hasher::hash(path.c_str()), path.c_str(), std::forward<Args>(args)...);
		}

		template<IsResource T, typename... Args>
		T& loadResourceImmediate(const char* path, Args&&... args)
		{
			Hash hash = Hasher::hash(path);

			std::unordered_map<Hash, IResource*>& map = resourceMaps_.at(T::index());

			if (!map.contains(hash))
				registerResource<T, Args...>(path, std::forward<Args>(args)...);

			auto& r = getResource<T>(path);
			r.load();
			return r;
		}

		template<IsResource T, typename... Args>
		T& loadResourceImmediate(const std::string& path, Args&&... args)
		{
			return loadResourceImmediate<T, Args...>(path.c_str(), std::forward<Args>(args)...);
		}

		template<IsResource T>
		T& getResource(const char* path)
		{
			Hash hash = Hasher::hash(path);

			std::unordered_map<Hash, IResource*>& map = resourceMaps_.at(T::index());

			if (!map.contains(hash))
				throw TraceException("Could not find resource! (maybe not registered?)");

			return *static_cast<T*>(map.at(hash));
		}

		template<IsResource T>
		T& getResource(const std::string& path)
		{
			return getResource<T>(path.c_str());
		}

		template<IsResource T>
		T& getResource(const std::filesystem::path& path)
		{
			return getResource<T>(path.string().c_str());
		}

		void loadResources();

	private:
		std::vector<IResource*> pendingResources_;
		std::vector<std::unordered_map<Hash, IResource*>> resourceMaps_;
	};
}