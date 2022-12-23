#pragma once

#include "pch.hpp"
#include "ComponentInfo.hpp"
#include "Hash.hpp"

namespace DarkDescent
{
	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager();

		template<typename T>
		inline const ComponentInfo& registerComponent()
		{
			const auto h = Hasher::hash(typeid(T).name());
			components_.insert({h, ComponentInfo(sizeof(T), components_.size())});
			return components_.at(h);
		}

	private:
		std::unordered_map<Hash, ComponentInfo> components_;
	};
}