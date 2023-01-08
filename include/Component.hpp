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
		Component() { }

		static std::size_t index;
		static std::size_t bitmask;
		static bool isInitialized;

	protected:
		friend class ArchManager;
		friend class Arch;
		friend class GameObject;

		template<class... T>
		friend std::size_t getComponentsBitmask<T>();
		template<class... T>
		friend std::vector<std::size_t> getComponentsBitmasks<T>();
	};

	template <class... T>
	std::size_t getComponentsBitmask()
	{
		std::vector<std::size_t> bitmasks = { (T::bitmask)... };
		std::size_t b = 0;
		for (auto b_ : bitmasks)
			b |= b_;
		return b;
	}

	template <class... T>
	std::vector<std::size_t> getComponentsBitmasks()
	{
		return { (T::bitmask)... };
	}

	

	template<typename T>
	std::size_t Component<T>::index = 0;

	template<typename T>
	std::size_t Component<T>::bitmask = 0;

	template<typename T>
	bool Component<T>::isInitialized = false;

	template<typename T>
	concept ExtendsComponent = std::is_base_of_v<Component<T>, T>;
}