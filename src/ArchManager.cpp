#include "ArchManager.hpp"
#include "Arch.hpp"
#include "ComponentInfo.hpp"
#include "Logger.hpp"
#include "TraceException.hpp"

namespace DarkDescent
{
	ArchManager::ArchManager():
		arches_({ { 0, new Arch(*this, {}) } })
	{ }

	ArchManager::~ArchManager()
	{

	}

	Arch& ArchManager::getArch(std::size_t bitmask)
	{
		if (!arches_.contains(bitmask))
		{
			std::string str = std::format("Could not get arch with bitmask {}", bitmask);
			throw TraceException(str.c_str());
		}
		return *arches_[bitmask];
	}

	Arch& ArchManager::getArch(const std::vector<const ComponentInfo*>& components)
	{
		auto bitmask = ComponentInfo::bitmaskFromComponents(components);
		Logger::get().debug("get arch bitmask: ", bitmask);
		if (!arches_.contains(bitmask))
			arches_.insert({ bitmask, new Arch(*this, components) });
		return *arches_[bitmask];
	}
}