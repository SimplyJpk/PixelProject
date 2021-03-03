#pragma once
#include "SDL.h"
#include "Vec2.h"
#include <backends/imgui_impl_sdl.h>

#include "InputKeyCodes.h"
//TODO Implement an event system? Would be nice to call events from here

class InputManager
{
public:
   static InputManager* Instance();

   void Update();

   bool IsAnyKeyDown();
   bool IsMovementKeysDown();

   // Mouse
   // Pressed this frame
   bool GetMouseDown(short button);
   // Released this frame
   bool GetMouseUp(short button);
   // State of mouse button (Down/Up)
   bool GetMouseButton(short button) const;

   // Keyboard
   // If the key is being pressed during this frame. (First, or ongoing)
   bool IsKeyHeld(KeyCode key_code);
   // Pressed this frame
   bool GetKeyDown(KeyCode key_code);
   // Released this frame
   bool GetKeyUp(KeyCode key_code);
   // State of key (Down/Up)
   bool GetKeyButton(KeyCode key_code) const;

   // Helper Methods
   int MouseX() const { return mouse_x_; }
   int MouseY() const { return mouse_y_; }
   IVec2 MousePosition() const { return mouse_pos_; }

   InputManager();

   InputManager(InputManager const&)
   {
   }

   void operator=(InputManager const&) const
   {
   }

   bool IsShuttingDown() { return GetKeyDown(KeyCode::Escape); };
protected:
   static inline bool IsValidKey(KeyCode& key_code);
private:
   static InputManager* instance_;

   const Uint8* keyboard_ = nullptr;
   uint32_t mouse_ = 0;

   int mouse_x_ = 0, mouse_y_ = 0;
   IVec2 mouse_pos_ = IVec2::Zero();
   IVec2 mouse_scroll_ = IVec2::Zero();

   bool is_movement_down_ = false;
   bool is_any_key_down_ = false;

   bool is_key_held_[keycode_max_value]{ false };

   bool is_key_down_[keycode_max_value]{ false };
   bool is_key_up_[keycode_max_value]{ false };

   bool is_mouse_down_[MouseClickTypeCount]{ false };
   bool is_mouse_up_[MouseClickTypeCount]{ false };
};
