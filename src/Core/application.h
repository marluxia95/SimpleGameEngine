/**
 * @file application.h
 * @author marlxx (marlxx@protonmail.com)
 * @brief This contains all the main application logic, loops, etc.
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <memory>
#include <thread>
#include "lynx_common.h"

namespace Lynx {

	class Renderer;
	class FontManager;

	class ThreadPool;
	class WindowManager;
	class ResourceManager;
	class EntityManager;

	enum State{
		STATE_ACTIVE,
		STATE_PAUSED,
		STATE_CLOSED
	};

	enum {
		APPLICATION_FULLSCREEN,
		APPLICATION_BORDERLESS = 1 << 1,
		APPLICATION_VSYNC = 1 << 2
	};

	class ResourceManager;

	class LYNXENGINE_API Application {
	public:
		Application();
		~Application();

		virtual void Initialize(int flags);

		virtual void Run();

		float GetDeltaTime() { return delta_time; }

		void SetApplicationState(State state) { applicationState = state; }

		std::shared_ptr<ResourceManager> GetResourceManager() { return m_resourceManager; }

		std::thread::id GetThread() { return thread_id; };

		void SetRenderer(std::shared_ptr<Renderer> renderer);
		std::shared_ptr<Renderer> GetRenderer() { return m_renderer; }

		std::shared_ptr<FontManager> GetFontManager() { return m_fontManager; }
		unsigned int GetResolutionHeight();
		unsigned int GetResolutionWidth();

		static Application* GetSingleton() { return s_applicationInstance; }
	protected:
		void CalculateFrameTime();

	public:
		friend class ResourceManager;
	private:
		static Application* s_applicationInstance;
	protected:
		float delta_time, last_FrameTime = 0.0f;
		State applicationState;
		std::thread::id thread_id;
		std::unique_ptr<ThreadPool> m_threadPool;	
		std::unique_ptr<WindowManager> m_windowManager;
		std::shared_ptr<FontManager> m_fontManager;
		std::shared_ptr<ResourceManager> m_resourceManager;
		std::shared_ptr<Renderer> m_renderer;
		std::shared_ptr<EntityManager> m_entityManager;

	};

}


#endif
