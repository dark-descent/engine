#pragma once

#define COMPONENT_CTOR(NAME) NAME() : Component()

namespace DarkDescent
{
	struct ComponentInfo
	{
		std::size_t size;
		std::size_t index;
		std::size_t bitmask;
	};

	template<typename T>
	struct Component
	{
	public:
		Component() {}

	protected:
		static std::size_t index;
		static std::size_t bitmask;
		static bool isInitialized;

		friend class ArchManager;
		friend class GameObject;
	};

	template<typename T>
	std::size_t Component<T>::index = 0;

	template<typename T>
	std::size_t Component<T>::bitmask = 0;
		
	template<typename T>
	bool Component<T>::isInitialized = false;
	

	template<typename T>
	concept ExtendsComponent = std::is_base_of_v<Component<T>, T>;
}