#include "ArchManager.hpp"
#include "Transform.hpp"
#include "ScriptManager.hpp"
#include "js/Transform.hpp"

namespace DarkDescent
{
	void ArchManager::onInitialize()
	{
		registerComponent<Transform>();
		addEventHandler(ScriptManager::Events::ENV_CREATED, [](SubSystem* self, const Event& e, void* data)
		{
			const JS::Env& env = *static_cast<const JS::Env*>(e.data);
			env.registerClass<JS::TransformClass>();
		}, this);
	}

	void ArchManager::onReady()
	{

	}

	void ArchManager::onTerminate()
	{

	}

	void ArchManager::registerArch(Bitmask bitmask, std::uint8_t mapIndex)
	{
		assert(mapIndex <= 1);
		auto& map = archMap_.at(mapIndex);
		assert(!map.contains(bitmask));

		std::vector<ComponentInfo*> c;
		std::size_t size = 0;

		for (std::size_t i = 0; i < bitmask; i++)
		{
			std::size_t b = 1ULL << i;
			if ((bitmask & b) == b)
			{
				ComponentInfo& component = components_.at(i);
				size += component.size;
				c.emplace_back(std::addressof(component));
			}
		}

		map.emplace(std::piecewise_construct, std::forward_as_tuple(bitmask), std::forward_as_tuple(*this, bitmask, size, std::move(c)));
	}

	void ArchManager::registerArch(Bitmask bitmask)
	{
		registerArch(bitmask, activeArchIndex_);
	}

	Arch* ArchManager::getArch(Bitmask bitmask, std::uint8_t mapIndex)
	{
		assert(mapIndex <= 1);
		auto& map = archMap_.at(mapIndex);
		if (!map.contains(bitmask))
			registerArch(bitmask, mapIndex);
		return std::addressof(map.at(bitmask));
	}

	Arch* ArchManager::getArch(Bitmask bitmask)
	{
		return getArch(bitmask, activeArchIndex_);
	}

	Entity ArchManager::allocEntity(Bitmask bitmask, std::uint8_t mapIndex)
	{
		assert(mapIndex <= 1);
		return getArch(bitmask, mapIndex)->alloc();
	}

	Entity ArchManager::allocEntity(Bitmask bitmask)
	{
		return getArch(bitmask)->alloc();
	}

	void ArchManager::swapActiveIndex()
	{
		activeArchIndex_ = !activeArchIndex_;
	}
}