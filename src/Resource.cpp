#include "Resource.hpp"

namespace DarkDescent
{
	Resource::Resource(std::string&& path):
		path(path),
		isLoaded_(false)
	{

	}

	Resource::Resource(const std::string& path):
		path(path),
		isLoaded_(false)
	{

	}

	Resource::~Resource()
	{

	}

	bool Resource::isLoaded() const
	{
		return isLoaded_;
	}

	bool Resource::load()
	{
		return true;		
	}
}