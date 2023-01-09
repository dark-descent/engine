#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"
#include "Renderer.hpp"
#include "PersistentVector.hpp"
#include "Task.hpp"

namespace DarkDescent
{
	class ArchManager;
	class TaskScheduler;
	class ArchBuffer;

	class RenderSystem: public SubSystem
	{
		SUB_SYSTEM_CTORS(RenderSystem),
			gameRenderer_(nullptr)
		{ }

	protected:
		virtual void onInitialize() override;
		virtual void onReady() override;
		virtual void onTerminate() override;

		using EventCallback = void(*)(RenderSystem&, const Event&);

		void handleEvent(Hash hash, EventCallback callback);
	
	public:
		Renderer* gameRenderer() const noexcept { return gameRenderer_; }

	private:
		static void onCreate(RenderSystem& system, const Event& event);
		static void onDestroy(RenderSystem& system, const Event& event);

		Task<void> renderSprites(ArchBuffer& buffer, const std::size_t transform, const std::size_t spriteRenderer);
		Task<void> render(std::size_t frame, TaskScheduler& scheduler);

		std::vector<Renderer*> renderers_;
		Renderer* gameRenderer_;
		ArchManager* archManager_;
	};
}