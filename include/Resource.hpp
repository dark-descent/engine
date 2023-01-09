#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	class ResourceManager;

	class IResource
	{
	public:
		explicit IResource(const std::string& path);
		virtual ~IResource() = 0;

		inline void load() {  assert(!isLoaded()); onLoad(); isLoaded_ = true; };

		virtual void onLoad() = 0;

		inline bool isLoaded() const { return isLoaded_; }
		inline void* data() const { assert(isLoaded()); return data_; }

		const std::string path;
	
	protected:
		bool isLoaded_;
		void* data_;

		friend class ResourceManager;
	};

	template<typename T, typename Data>
	class Resource : public IResource
	{
	private:
		static std::size_t index_;
		static std::size_t isRegistered_;

	public:
		static inline std::size_t index() { return index_; }
		static inline std::size_t isRegistered() { return isRegistered_; }

	public:
		explicit Resource(const std::string& path) : IResource(path) {};
		virtual ~Resource() {};

		inline char* raw() const { return static_cast<char*>(IResource::data()); }
		inline Data& data() const { return *static_cast<Data*>(IResource::data()); }
	
	protected:
		virtual void onLoad() = 0;
		
		friend class ResourceManager;
	};

	template<typename T, typename D>
	std::size_t Resource<T, D>::index_ = 0;

	template<typename T, typename D>
	std::size_t Resource<T, D>::isRegistered_ = false;

	template<typename T>
	concept IsResource = std::is_base_of_v<IResource, T>;
}