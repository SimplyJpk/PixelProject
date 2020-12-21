#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"
#include "InputManager.h"

class PaintManager
{
public:
		short SelectedPixelType = 0;
		BasePixel* SelectedPixel = nullptr;

		const char* SelectedPixelName() { return SelectedPixel->Name();  }

		void UpdateInput() {
				InputManager* input = InputManager::Instance();
				if (input->GetKeyDown(KeyCode::Tab)) {
						if (SelectedPixel->PixelIndex == m_worldData->PixelTypeCount() - 1)
								SelectedPixel = m_worldData->GetPixelFromIndex(1);
						else
								SelectedPixel = m_worldData->GetPixelFromIndex(SelectedPixel->PixelIndex + 1);
				}
		}

		PaintManager() {
				m_worldData = WorldDataHandler::Instance();
				SelectedPixel = m_worldData->GetPixelFromIndex(1);
		}
private:
		WorldDataHandler* m_worldData = nullptr;
};