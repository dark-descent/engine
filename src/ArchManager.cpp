#include "ArchManager.hpp"
#include "Arch.hpp"

namespace DarkDescent
{
	ArchManager::ArchManager():
		rootArch_(new Arch(*this, 0, 0, 0, {}))
	{

	}

	ArchManager::~ArchManager()
	{
		delete rootArch_;
	}
}