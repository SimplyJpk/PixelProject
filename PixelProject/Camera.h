#pragma once
#include "GameSettings.h"
#include "GameObject.h"

class Camera final : public GameObject
{
public:
		SDL_Rect view_port{};

		explicit Camera(GameSettings* settings) {
				SetScreenSize(settings->screen_size);
		}
		Camera(const IVec2& position, GameSettings* settings) {
				SetScreenSize(settings->screen_size);
				SetPosition(position);
		}
		
		void Start() override;
		void Update() override;

		void SetScreenSize(const IVec2& size) {
				view_port.h = size.y;
				view_port.w = size.x;
		}
		void SetPosition(const IVec2& pos) {
				view_port.x = pos.x;
				view_port.y = pos.y;
		}
private:
};

