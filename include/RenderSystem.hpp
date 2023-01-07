#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"
#include "Renderer.hpp"
#include "PersistentVector.hpp"

namespace DarkDescent
{
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

	private:
		static void onCreate(RenderSystem& system, const Event& event);
		static void onDestroy(RenderSystem& system, const Event& event);
		static void onRender(RenderSystem& system, const Event& event);

		PersistentVector<Renderer, 2> renderers_;
		Renderer* gameRenderer_;
	};
}