#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <GLFW/glfw3.h>

namespace Lynx {

    class WindowManager {
		public:
			void Init(const char* title = "SimpleGameEngine", unsigned int width = 1270, unsigned int height = 720, bool fullScreen = false);
			
            void Update();
			
            void Destroy();

			unsigned int window_width = 1280;
			unsigned int window_height = 720;
			GLFWwindow* window;
	};


}

#endif