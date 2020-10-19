#include "Camera.h"
#include "InputHandler.h"

void Camera::Start()
{
}

void Camera::Update()
{
}

void Camera::SubscribeInputs(InputHandler* inputHandler)
{
		inputHandler->Subscribe(this, InputEvent::ANY_MOVEMENT);
}

void Camera::UpdateInput(const int eventType, const SDL_Event* _event)
{
		switch (_event->key.keysym.sym)
		{
		case SDLK_LEFT:
		case SDLK_a:
				viewPort.x -= 5;
				break;
		case SDLK_RIGHT:
		case SDLK_d:
				viewPort.x += 5;
				break;
		case SDLK_UP:
		case SDLK_w:
				viewPort.y += 5;
				break;
		case SDLK_DOWN:
		case SDLK_s:
				viewPort.y -= 5;
				break;
		default:
				break;
		}
}