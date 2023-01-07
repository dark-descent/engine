#include "RenderSystem.hpp"
#include "Engine.hpp"
#include "TraceException.hpp"
#include "WindowManager.hpp"
#include "Logger.hpp"
#include "ArchManager.hpp"
#include "SpriteRenderer.hpp"
#include "Transform.hpp"
#include "TaskScheduler.hpp"

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
		archManager_ = engine_.getSubSystem<ArchManager>();
	}

	void RenderSystem::onInitialize()
	{
		handleEvent(Hasher::hash("WINDOW_CREATED"), onCreate);
		handleEvent(Hasher::hash("WINDOW_DESTROY"), onDestroy);
		// handleEvent(Hasher::hash("WINDOW_RESIZED"), onRender); // TODO: rerender the screen
	}

	void RenderSystem::onTerminate()
	{

	}

	void RenderSystem::onCreate(RenderSystem& system, const Event& event)
	{
		const std::size_t index = *static_cast<const std::size_t*>(event.data);
		auto& w = system.engine_.getSubSystem<WindowManager>()->getWindow(index);
		auto renderer = system.renderers_.emplace_back(new Renderer(std::addressof(w)));
		if (w.isGameWindow())
		{
			assert(system.gameRenderer_ == nullptr);
			system.gameRenderer_ = renderer;
		}
	}

	void RenderSystem::onDestroy(RenderSystem& system, const Event& event)
	{

	}

	Task<void> RenderSystem::renderSprites(ArchBuffer& buffer, const std::size_t transformOffset, const std::size_t spriteOffset)
	{
		buffer.iterate([&](const ArchBuffer::Data& data)
		{
			auto& transform = data.get<Transform>(transformOffset);
			auto& spriteRenderer = data.get<SpriteRenderer>(spriteOffset);
		});
		co_return;
	}

	Task<void> RenderSystem::render(std::size_t frame, TaskScheduler& scheduler)
	{
		assert(gameRenderer_ != nullptr);
		assert(archManager_ != nullptr);
		std::vector<Arch*> arches = archManager_->queryArches<Transform, SpriteRenderer>();
		std::vector<Task<>> tasks;
		for(auto arch : arches)
		{
			const std::size_t transformOffset = arch->getComponentOffset<Transform>().offset;
			const std::size_t spriteOffset = arch->getComponentOffset<SpriteRenderer>().offset;
			
			for(auto buf : arch->bufferPool().buffers())
			{
				tasks.emplace_back(renderSprites(*buf, transformOffset, spriteOffset));
			}
		}
		
		gameRenderer_->render();
		co_await scheduler.awaitTasks(tasks);
		co_return;
	}
}