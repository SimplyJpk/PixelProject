#pragma once
#include "BasePixel.h"
#include "WorldDataHandler.h"
#include "InputManager.h"

class PaintManager
{
public:
   short selected_pixel_type = 0;
   BasePixel* selected_pixel = nullptr;

   const char* SelectedPixelName() const { return selected_pixel->Name(); }

   void UpdateInput()
   {
      auto* input = InputManager::Instance();
      if (input->GetKeyDown(KeyCode::Tab))
      {
         if (selected_pixel->pixel_index == world_data_->PixelTypeCount() - 1)
            selected_pixel = world_data_->GetPixelFromIndex(1);
         else
            selected_pixel = world_data_->GetPixelFromIndex(selected_pixel->pixel_index + 1);
      }
   }

   PaintManager()
   {
      world_data_ = WorldDataHandler::Instance();
      //TODO Init this somewhere else, this could force instant ordering problems.
      selected_pixel = world_data_->GetPixelFromIndex(1);
   }

private:
   WorldDataHandler* world_data_ = nullptr;
};
