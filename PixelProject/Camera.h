#pragma once
#include "GameSettings.h"
#include "GameObject.h"

class Camera : public GameObject
{
public:
		SDL_Rect viewPort;

		Camera(GameSettings* settings) {
				SetScreenSize(settings->_CONFIG_SCREEN_SIZE);
		}
		Camera(IVec2 position, GameSettings* settings) {
				SetScreenSize(settings->_CONFIG_SCREEN_SIZE);
				SetPosition(position);
		}
		
		void Start() override;
		void Update() override;

		void SetScreenSize(IVec2 size) {
				viewPort.h = size.y;
				viewPort.w = size.x;
		}
		void SetPosition(IVec2 pos) {
				viewPort.x = pos.x;
				viewPort.y = pos.y;
		}

		void SubscribeInputs(InputHandler* inputHandler) override;
		void UpdateInput(const int eventType, const SDL_Event* _event) override;

private:
		Camera();
};

