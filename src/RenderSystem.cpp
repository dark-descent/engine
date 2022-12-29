#include "RenderSystem.hpp"
#include "Engine.hpp"
#include "TraceException.hpp"
#include "WindowManager.hpp"

namespace DarkDescent
{
	void RenderSystem::handleEvent(Hash hash, EventCallback callback)
	{
		addEventHandler(hash, [](SubSystem* self, const Event& event, void* data)
		{
			RenderSystem& system = *static_cast<RenderSystem*>(self);
			EventCallback callback = reinterpret_cast<EventCallback>(data);
			callback(system, event);
		}, callback);
	}

	void RenderSystem::onReady()
	{

	}

	void RenderSystem::onInitialize()
	{
		handleEvent(Hasher::hash("WINDOW_CREATED"), onCreate);
		handleEvent(Hasher::hash("WINDOW_DESTROY"), onDestroy);
		handleEvent(Hasher::hash("WINDOW_RESIZED"), onRender);
		handleEvent(Hasher::hash("RENDER"), onRender);
	}

	void RenderSystem::onTerminate()
	{

	}

	void RenderSystem::onCreate(RenderSystem& system, const Event& event)
	{
		Logger::get().info("Window created!");
		const std::size_t index = *static_cast<const std::size_t*>(event.data);
		system.renderers_.emplace_at(index, &system.engine_.getSubSystem<WindowManager>()->getWindow(index));
	}

	void RenderSystem::onDestroy(RenderSystem& system, const Event& event)
	{
		Logger::get().info("Window destroy!");
	}

	void RenderSystem::onRender(RenderSystem& system, const Event& event)
	{
		system.renderers_.at(*static_cast<const std::size_t*>(event.data)).render();
		Logger::get().info("render!");
	}
}