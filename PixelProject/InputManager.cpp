#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager() {}

InputManager* InputManager::Instance() {
		if (!instance)
				instance = new InputManager();
		return instance;
}

void InputManager::Update() {
		// We clear all our states
		// Keyboard
		SDL_memset(isKeyDown, false, SCANCODE_MAXSIZE * sizeof(bool));
		SDL_memset(isKeyUp, false, SCANCODE_MAXSIZE * sizeof(bool));
		// Mouse
		SDL_memset(isMouseDown, false, MouseClickTypeCount * sizeof(bool));
		SDL_memset(isMouseUp, false, MouseClickTypeCount * sizeof(bool));

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
				switch (event.type)
				{
				case SDL_KEYDOWN:
						keyboard = SDL_GetKeyboardState(nullptr);
						isKeyDown[event.key.keysym.scancode] = true;
						break;
				case SDL_KEYUP:
						keyboard = SDL_GetKeyboardState(nullptr);
						isKeyUp[event.key.keysym.scancode] = true;
				case SDL_MOUSEMOTION:
						mouseX = event.motion.x;
						mouseY = event.motion.y;
						mousePos = IVec2(mouseX, mouseY);
						break;
				case SDL_MOUSEBUTTONDOWN:
						mouse = SDL_GetMouseState(&(mouseX), &(this->mouseY));
						if (event.button.button == SDL_BUTTON_LEFT)
								isMouseDown[MouseLeft] = true;
						else if (event.button.button == SDL_BUTTON_RIGHT)
								isMouseDown[MouseRight] = true;
						else if (event.button.button == SDL_BUTTON_MIDDLE)
								isMouseDown[MouseMiddle] = true;
						break;

				case SDL_MOUSEBUTTONUP:
						mouse = SDL_GetMouseState(&(mouseX), &(this->mouseY));
						if (event.button.button == SDL_BUTTON_LEFT)
								isMouseUp[MouseLeft] = true;
						else if (event.button.button == SDL_BUTTON_RIGHT)
								isMouseUp[MouseRight] = true;
						else if (event.button.button == SDL_BUTTON_MIDDLE)
								isMouseUp[MouseMiddle] = true;
						break;

				case SDL_MOUSEWHEEL:
						//TODO Does this work?
						mouseScroll.x = event.wheel.x;
						mouseScroll.y = event.wheel.y;
						break;
				case SDL_QUIT:
						// TODO Should do something for this
						break;
				default:
						break;
				}
		}
}

bool InputManager::GetMouseDown(short button)
{
		if (button < 0 || button >= MouseClickTypeCount)
				return false;
		return (isMouseDown[button]);
}

bool InputManager::GetMouseUp(short button)
{
		if (button < 0 || button >= MouseClickTypeCount)
				return false;
		return (isMouseUp[button]);
}

bool InputManager::GetMouseButton(short button)
{
		switch (button)
		{
		case MouseLeft:
				if (mouse & SDL_BUTTON(1))
						return true;
				break;
		case MouseMiddle:
				if (mouse & SDL_BUTTON(2))
						return true;
				break;
		case MouseRight:
				if (mouse & SDL_BUTTON(3))
						return true;
				break;
		default:
				return false;
		}
		return false;
}

bool InputManager::GetKeyDown(KeyCode keyCode)
{
		int key = static_cast<int>(keyCode);
		if (key < 0 || key >= SCANCODE_MAXSIZE)
				return false;
		return (isKeyDown[key]);
}

bool InputManager::GetKeyUp(KeyCode keyCode)
{
		int key = static_cast<int>(keyCode);
		if (key < 0 || key >= SCANCODE_MAXSIZE)
				return false;
		return (isKeyUp[key]);
}

bool InputManager::GetKeyButton(KeyCode keyCode)
{
		if (!(keyboard))
				return false;
		if (keyboard[static_cast<int>(keyCode)])
				return true;
		return false;
}