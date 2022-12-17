#pragma once

namespace DarkDescent
{
	class Arch;
	class ComponentManager;

	class ArchManager
	{
	public:
		ArchManager();
		~ArchManager();

		inline Arch& rootArch() const { return *rootArch_; }
		
	private:
		Arch* rootArch_;
	};
}