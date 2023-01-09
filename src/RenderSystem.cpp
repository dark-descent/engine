#include "RenderSystem.hpp"
#include "Engine.hpp"
#include "TraceException.hpp"
#include "WindowManager.hpp"
#include "Logger.hpp"
#include "ArchManager.hpp"
#include "SpriteRenderer.hpp"
#include "Transform.hpp"
#include "TaskScheduler.hpp"
#include "ScriptManager.hpp"
#include "js/SpriteRenderer.hpp"

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
		auto renderer = system.renderers_.emplace_back(new Renderer(system, std::addressof(w)));
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
		glEnableVertexAttribArray(0);

		buffer.iterate([ & ](const ArchBuffer::Data& data)
		{
			auto& transform = data.get<Transform>(transformOffset);
			auto& spriteRenderer = data.get<SpriteRenderer>(spriteOffset);

			glBindBuffer(GL_ARRAY_BUFFER, spriteRenderer.vbo);

			glVertexAttribPointer(
				0,			// attribute must match shader layout
				3,			// size
				GL_FLOAT,	// type
				GL_FALSE,	// normalized?
				0,			// stride
				(void*)0 	// array buffer offset
			);

			glDrawArrays(GL_TRIANGLES, 0, 18);
		});

		glDisableVertexAttribArray(0);

		co_return;
	}

	Task<> RenderSystem::render(std::size_t frame, TaskScheduler& scheduler)
	{
		assert(gameRenderer_ != nullptr);
		assert(archManager_ != nullptr);

		auto queries = archManager_->getArchQueries<Transform, SpriteRenderer>();

		std::vector<Task<>> tasks;

		for (auto& query : queries)
		{
			auto transformOffset = query.getOffset<Transform>();
			auto spriteOffset = query.getOffset<SpriteRenderer>();
			tasks.emplace_back(renderSprites(*query.buffer, transformOffset, spriteOffset));
		}

		gameRenderer_->renderSetup();

		if (tasks.size() > 0)
			co_await scheduler.awaitTasks(tasks);

		gameRenderer_->render();

		co_return;
	}
}