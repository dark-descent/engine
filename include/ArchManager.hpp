#pragma once

#include "pch.hpp"

namespace DarkDescent
{
	class Arch;
	class ComponentManager;
	class ComponentInfo;

	class ArchManager
	{
	public:
		ArchManager();
		~ArchManager();

		Arch& getArch(const std::vector<const ComponentInfo*>& components);
		Arch& getArch(std::size_t bitmask);
		
		inline Arch& rootArch() { return *arches_[0ULL]; }
		
	private:
		std::unordered_map<std::uint64_t, Arch*> arches_;
	};
}