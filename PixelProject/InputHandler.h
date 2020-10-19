#pragma once
#include "InputEventTypes.h"
#include <SDL_events.h>
#include <vector>
#include "LinkedList.h"

#include "GameObject.h"

class InputHandler
{
public:
		InputHandler();

		void HandleInput();

		bool Subscribe(GameObject* inputMethod, InputEvent input);
		bool Unsubscribe(GameObject* inputMethod, InputEvent input);

		std::vector < LinkedList<GameObject>> InputEvents;
protected:
		void CallEvents(InputEvent eventType, SDL_Event* _event = nullptr);
private:
		int mouseX, mouseY;
		bool leftClickDown = false;

		SDL_Event _inputEvent;
};


