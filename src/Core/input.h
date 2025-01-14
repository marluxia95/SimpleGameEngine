/**
 * @file input.h
 * @author marlxx (marlxx@protonmail.com)
 * @brief This file contains all the input handling, including gamepads.
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <stdbool.h>
#include <map>
#include <glm/glm.hpp>
#include "lynx_common.h"
#include "keys.h"

#define JOYSTICK_LAST 15

namespace Lynx {

	class Event;

	struct gamepadstate
	{
		unsigned char buttons[15];
		float axes[6];
	};

	struct Joystick
	{
		int id;
		const char* name;
		int axisCount;
		int buttonCount;
		int hatCount;
		bool isGamePad;
		gamepadstate gpadstate;
		void* ptr;
	};

	class Application;

	class LYNXENGINE_API Input {
		public:
			~Input();
			static bool IsJoystickConnected(int joyid);
			static float GetJoyAxis(int joyid, int axis);
			static bool GetJoyButton(int joyid, int axis);
			static bool IsKeyDown(int keycode);
			static bool IsKeyUp(int keycode);
			static glm::dvec2 GetMousePos();
			static void EnableCursor(bool enable);

			friend class Application;
		private:
			static void Init();
			static void joystick_callback(int id, int ev);
			static void mouse_callback(const Event& ev);
			static void UpdateKeys(const Event& ev);
			static int keysUp[KEY_LAST];
			static int keysDown[KEY_LAST];
			static int isJoystickPresent;

			static void addJoystick(int id);
			static Joystick* connectedJoysticks[JOYSTICK_LAST];

			static glm::dvec2 mouse_pos;
	};

}

#endif
