#pragma once
#include "SDL.h"
#include "Vec2.h"

#include "InputKeyCodes.h"
//TODO Implement an event system? Would be nice to call events from here

class InputManager
{
public:
		static InputManager* Instance();

		void Update();

		// Mouse
		// Pressed this frame
		bool GetMouseDown(short button);
		// Released this frame
		bool GetMouseUp(short button);
		// State of mouse button (Down/Up)
		bool GetMouseButton(short button);

		// Keyboard
		// Pressed this frame
		bool GetKeyDown(KeyCode keyCode);
		// Released this frame
		bool GetKeyUp(KeyCode keyCode);
		// State of key (Down/Up)
		bool GetKeyButton(KeyCode keyCode);

		// Helper Methods
		int MouseX() { return mouseX; }
		int MouseY() { return mouseY; }
		IVec2 MousePosition() { return mousePos; }

		InputManager();

		InputManager(InputManager const&) {};
		void operator=(InputManager const&) {};
private:
		static InputManager* instance;

		const Uint8* keyboard = nullptr;
		uint32_t mouse = 0;

		int mouseX = 0, mouseY = 0;
		IVec2 mousePos = IVec2::Zero();
		IVec2 mouseScroll = IVec2::Zero();

		bool isKeyDown[SCANCODE_MAXSIZE]{ false };
		bool isKeyUp[SCANCODE_MAXSIZE]{ false };

		bool isMouseDown[MouseClickTypeCount]{ false };
		bool isMouseUp[MouseClickTypeCount]{ false };

};
