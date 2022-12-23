#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"
#include "Hash.hpp"
#include "TraceException.hpp"
#include "Arch.hpp"
#include "Component.hpp"

namespace DarkDescent
{	
	template<typename T>
	concept ComponentType = requires(T)
	{
		T::index;
		T::bitmask;
	};

	class ArchManager: public SubSystem
	{
		using Bitmask = std::size_t;
		using ComponentSize = std::size_t;

	public:
		SUB_SYSTEM_CTORS(ArchManager),
			componentCounter_(0),
			registeredComponents_(),
			components_()
		{}

		bool isComponentRegistered(const Hash h)
		{
			for (const auto& h_ : registeredComponents_)
			{
				if (h == h_)
					return true;
			}
			return false;
		}

		template<ComponentType T>
		const Component& registerComponent()
		{
			Hash h = Hasher::hash(typeid(T).name());

			assert(!isComponentRegistered(h));

			std::size_t index = componentCounter_++;

			registeredComponents_[index] = h;

			const std::size_t bitmask = 1ULL << index;

			components_[index] = {
				.size = sizeof(T),
				.index = index,
				.bitmask = bitmask,
			};

			T::index = index;
			T::bitmask = bitmask;

			return components_[index];
		}

		template<typename T>
		const Component& getComponent()
		{
			Hash h = Hasher::hash(typeid(T).name());

			for (std::size_t i = 0, l = registeredComponents_.size(); i < l; i++)
			{
				if (registeredComponents_[i] == h)
					return components_[i];
			}

			std::string error = std::format("Could not get component for type {}!", typeid(T).name());

			throw TraceException(error.c_str());
		}

		template<typename T>
		inline Bitmask getComponentBitmask()	
		{
			return getComponent<T>().bitmask;
		}

		Arch* getArch(Bitmask bitmask);
		void registerArch(Bitmask bitmask);

		inline Arch* rootArch() { return getArch(0); }

		Entity allocEntity(Bitmask bitmask);

	protected:
		virtual void onInitialize() override;
		virtual void onTerminate() override;

	private:
		std::size_t componentCounter_;
		std::array<Hash, 64> registeredComponents_;
		std::array<Component, 64> components_;

		std::unordered_map<Bitmask, Arch> arches_;
	};
}