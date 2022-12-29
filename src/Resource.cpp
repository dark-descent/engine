#include "Resource.hpp"

namespace DarkDescent
{
	IResource::IResource(const std::string& path):
		path(path),
		isLoaded_(false)
	{

	}

	IResource::~IResource()
	{

	}
}