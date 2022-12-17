#pragma once

#include "ComponentInfo.hpp"

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
			return components_.emplace_back(sizeof(T), components_.size());
		}

	private:
		std::vector<ComponentInfo> components_;
	};
}