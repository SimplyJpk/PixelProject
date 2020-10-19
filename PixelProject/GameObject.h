#pragma once
#include <SDL.h>

class Camera;
class InputHandler;

class GameObject
{
		//TODO Make a physics Object?
public:
		SDL_Rect transform;

		virtual void Start() { };
		virtual void Update() { };
		virtual void Destroy() { };

		//TODO this could probably be made into its own interface?
		virtual void SubscribeInputs(InputHandler* inputHandler) {};
		virtual void UpdateInput(const int eventType, const SDL_Event* _event) { };

		//TODO Move this into a renderer class?
		// TODO Hopefully temp draw method?
		virtual bool Draw(Camera* camera) { return false; };
};

