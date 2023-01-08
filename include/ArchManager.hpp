#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"
#include "Hash.hpp"
#include "TraceException.hpp"
#include "Arch.hpp"
#include "Component.hpp"

namespace DarkDescent
{
	class ArchManager: public SubSystem
	{
		using Bitmask = std::size_t;
		using ComponentSize = std::size_t;

	public:
		constexpr static std::uint8_t UNSET_INDEX = std::numeric_limits<std::uint8_t>::max();

		SUB_SYSTEM_CTORS(ArchManager),
			componentCounter_(0),
			registeredComponents_(),
			components_(),
			activeArchIndex_(0),
			archMap_({  })
		{ }

		bool isComponentRegistered(const Hash h)
		{
			for (const auto& h_ : registeredComponents_)
			{
				if (h == h_)
					return true;
			}
			return false;
		}

		template<ExtendsComponent T>
		const ComponentInfo& registerComponent()
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
			T::isInitialized = true;

			return components_[index];
		}

		inline std::uint8_t getNextActiveIndex() const
		{
			return static_cast<std::uint8_t>(!activeArchIndex_);
		}

		inline std::uint8_t activeArchIndex() const
		{
			return static_cast<std::uint8_t>(!activeArchIndex_);
		}

		inline const ComponentInfo& getComponent(const std::size_t index)
		{
			return components_.at(index);
		}

		template<ExtendsComponent T>
		const ComponentInfo& getComponent()
		{
			assert(T::isInitialized);
			return components_.at(T::index);
		}

		Arch* getArch(Bitmask bitmask);
		Arch* getArch(Bitmask bitmask, std::uint8_t mapIndex);

		void registerArch(Bitmask bitmask);
		void registerArch(Bitmask bitmask, std::uint8_t mapIndex);

		inline Arch* rootArch() { return getArch(0); }
		inline Arch* rootArch(std::uint8_t mapIndex)
		{
			assert(mapIndex != UNSET_INDEX);
			return getArch(0, mapIndex);
		}

		Entity allocEntity(Bitmask bitmask);
		Entity allocEntity(Bitmask bitmask, std::uint8_t mapIndex);

		void swapActiveIndex();

		struct Query
		{
			template<typename... T>
			static Query create(Arch* arch, ArchBuffer* buffer)
			{
				return {
					arch,
					buffer
				};
			}

			template<typename T>
			std::size_t getOffset() const
			{
				return arch->getComponentOffset<T>().offset;
			}

			Arch* arch;
			ArchBuffer* buffer;
		};

		template<class... T>
		std::vector<Query> getArchQueries()
		{
			const Bitmask bitmask = getComponentsBitmask<T...>();

			std::vector<Query> queries;

			for (auto& [key, arch] : activeArchMap())
			{
				if ((key & bitmask) == bitmask)
				{
					auto archPtr = std::addressof(arch);
					for(auto buffer : arch.bufferPool().buffers())
						queries.emplace_back(Query::create<T...>(archPtr, buffer));
				}
			}

			return queries;
		}

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;

	private:
		std::unordered_map<Bitmask, Arch>& activeArchMap() { return archMap_.at(activeArchIndex()); };

		std::size_t componentCounter_;
		std::array<Hash, 64> registeredComponents_;
		std::array<ComponentInfo, 64> components_;

		bool activeArchIndex_;
		std::array<std::unordered_map<Bitmask, Arch>, 2> archMap_;
	};
}