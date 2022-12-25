#include "Scene.hpp"
#include "Resource.hpp"

namespace DarkDescent
{
	Scene::Scene(const std::string& path, const Resource& resource):
		path(path),
		resource(resource)
	{

	}

	Scene::~Scene()
	{

	}
	
	void Scene::load()
	{
		
	}
	
	void Scene::unload()
	{

	}
}