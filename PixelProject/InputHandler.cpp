#include "InputHandler.h"

InputHandler::InputHandler()
{
		InputEvents.resize(InputEventCount);
}

void InputHandler::HandleInput()
{
		bool leftClickUpdated = false;
		while (SDL_PollEvent(&_inputEvent)) {
				switch (_inputEvent.type)
				{
				case SDL_QUIT:
						CallEvents(InputEvent::USER_QUIT, &_inputEvent);
						break;
				case SDL_MOUSEBUTTONUP:
						leftClickDown = false;
						break;
				case SDL_MOUSEBUTTONDOWN:
						leftClickDown = true;
				case SDL_MOUSEMOTION:
						if (SDL_GetMouseState(&_inputEvent.motion.x, &_inputEvent.motion.y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
								CallEvents(InputEvent::USER_CLICK_DOWN, &_inputEvent);
								leftClickUpdated = true;
						}
				case SDL_KEYDOWN:
						// We fire any KeyBoard events listening for just a keyboard update (These may just use the event to determine what they're doing)
						CallEvents(InputEvent::RAW_KEYBOARD_INPUT, &_inputEvent);
						// Now we check what key was pressed, and forward individual keystrokes instead.
						SDL_KeyboardEvent key = _inputEvent.key;
						if (key.keysym.sym == SDLK_LEFT || key.keysym.sym == SDLK_a) {
								CallEvents(InputEvent::LEFT_MOVEMENT, &_inputEvent);
								CallEvents(InputEvent::ANY_MOVEMENT, &_inputEvent);
						}
						else if (key.keysym.sym == SDLK_RIGHT || key.keysym.sym == SDLK_d) {
								CallEvents(InputEvent::RIGHT_MOVEMENT, &_inputEvent);
								CallEvents(InputEvent::ANY_MOVEMENT, &_inputEvent);
						}
						else if (key.keysym.sym == SDLK_UP || key.keysym.sym == SDLK_w) {
								CallEvents(InputEvent::UP_MOVEMENT, &_inputEvent);
								CallEvents(InputEvent::ANY_MOVEMENT, &_inputEvent);
						}
						else if (key.keysym.sym == SDLK_DOWN || key.keysym.sym == SDLK_s) {
								CallEvents(InputEvent::DOWN_MOVEMENT, &_inputEvent);
								CallEvents(InputEvent::ANY_MOVEMENT, &_inputEvent);
						}
						break;
				default:
						break;
				}
		}
		if (leftClickDown && !leftClickUpdated) {
				CallEvents(InputEvent::USER_CLICK_DOWN, &_inputEvent);
		}
		//				switch (event.type)
		//				{
		//				case SDL_QUIT:
		//						quit = true;
		//						break;
		//				case SDL_MOUSEBUTTONUP:
		//						if (event.button.button == SDL_BUTTON_LEFT)
		//								leftMouseButtonDown = false;
		//						break;
		//				case SDL_MOUSEBUTTONDOWN:
		//						if (event.button.button == SDL_BUTTON_LEFT)
		//								leftMouseButtonDown = true;
		//				case SDL_MOUSEMOTION:
		//						if (leftMouseButtonDown)
		//						{
		//								mouseX = event.motion.x;
		//								mouseY = event.motion.y;
		//						}
		//						break;
		//						/* Keyboard event */
		//				case SDL_KEYDOWN:
		//						//case SDL_KEYUP:
		//						SDL_KeyboardEvent key = event.key;
		//						if (key.keysym.sym == SDLK_LEFT) {
		//								if (activeColour == red) activeColour = green;
		//								else if (activeColour == green) activeColour = blue;
		//								else if (activeColour == blue) activeColour = brown;
		//								else if (activeColour == brown) activeColour = red;
		//						}
		//				}
}

//TODO Improve this, can we check if an event already exists, how do we remove? should we wrap these?
bool InputHandler::Subscribe(GameObject* inputObject, InputEvent eventType = InputEvent::UNKNOWN)
{
		InputEvents[eventType].AddBackNode(inputObject);
		return true;
}

bool InputHandler::Unsubscribe(GameObject* inputObject, InputEvent eventType) {
		LinkedListNode<GameObject>* node = InputEvents[eventType].head;
		while (node != NULL) {
				if (node->Data == inputObject) {
						InputEvents[eventType].RemoveNode(node, false);
						return true;
				}
				node = node->Next;
		}
		return false;
}

void InputHandler::CallEvents(InputEvent eventType, SDL_Event* _event)
{
		LinkedListNode<GameObject>* node = InputEvents[eventType].head;
		while (node != NULL) {
				node->Data->UpdateInput(eventType, _event);
				node = node->Next;
		}
}

//Command* InputHandler::HandleInput()
//{
//		if (SDL_PollEvent(&event)) {
//				switch (event.type)
//				{
//				case SDL_QUIT:
//						quit = true;
//						break;
//				case SDL_MOUSEBUTTONUP:
//						if (event.button.button == SDL_BUTTON_LEFT)
//								leftMouseButtonDown = false;
//						break;
//				case SDL_MOUSEBUTTONDOWN:
//						if (event.button.button == SDL_BUTTON_LEFT)
//								leftMouseButtonDown = true;
//				case SDL_MOUSEMOTION:
//						if (leftMouseButtonDown)
//						{
//								mouseX = event.motion.x;
//								mouseY = event.motion.y;
//						}
//						break;
//						/* Keyboard event */
//				case SDL_KEYDOWN:
//						//case SDL_KEYUP:
//						SDL_KeyboardEvent key = event.key;
//						if (key.keysym.sym == SDLK_LEFT) {
//								if (activeColour == red) activeColour = green;
//								else if (activeColour == green) activeColour = blue;
//								else if (activeColour == blue) activeColour = brown;
//								else if (activeColour == brown) activeColour = red;
//						}
//				}
//		}
//		return nullptr;
//}
