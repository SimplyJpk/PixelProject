#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

InputManager::InputManager() = default;

InputManager* InputManager::Instance()
{
   if (!instance_)
      instance_ = new InputManager();
   return instance_;
}

void InputManager::Update()
{
   // We clear all our states
   // Keyboard
   SDL_memset(is_key_down_, false, SCANCODE_MAXSIZE * sizeof(bool));
   SDL_memset(is_key_up_, false, SCANCODE_MAXSIZE * sizeof(bool));
   // Mouse
   SDL_memset(is_mouse_down_, false, MouseClickTypeCount * sizeof(bool));
   SDL_memset(is_mouse_up_, false, MouseClickTypeCount * sizeof(bool));

   SDL_Event event;
   while (SDL_PollEvent(&event))
   {
      ImGui_ImplSDL2_ProcessEvent(&event);
      switch (event.type)
      {
      case SDL_KEYDOWN:
         keyboard_ = SDL_GetKeyboardState(nullptr);
         is_key_down_[event.key.keysym.scancode] = true;
         break;
      case SDL_KEYUP:
         keyboard_ = SDL_GetKeyboardState(nullptr);
         is_key_up_[event.key.keysym.scancode] = true;
      case SDL_MOUSEMOTION:
         //TODO May need to remove the +1
         mouse_x_ = event.motion.x + 1;
         mouse_y_ = event.motion.y + 1;
         mouse_pos_ = IVec2(mouse_x_, mouse_y_);
         break;
      case SDL_MOUSEBUTTONDOWN:
         mouse_ = SDL_GetMouseState(&(mouse_x_), &(this->mouse_y_));
         if (event.button.button == SDL_BUTTON_LEFT)
            is_mouse_down_[MouseLeft] = true;
         else if (event.button.button == SDL_BUTTON_RIGHT)
            is_mouse_down_[MouseRight] = true;
         else if (event.button.button == SDL_BUTTON_MIDDLE)
            is_mouse_down_[MouseMiddle] = true;
         break;

      case SDL_MOUSEBUTTONUP:
         mouse_ = SDL_GetMouseState(&(mouse_x_), &(this->mouse_y_));
         if (event.button.button == SDL_BUTTON_LEFT)
            is_mouse_up_[MouseLeft] = true;
         else if (event.button.button == SDL_BUTTON_RIGHT)
            is_mouse_up_[MouseRight] = true;
         else if (event.button.button == SDL_BUTTON_MIDDLE)
            is_mouse_up_[MouseMiddle] = true;
         break;

      case SDL_MOUSEWHEEL:
         //TODO Does this work?
         mouse_scroll_.x = event.wheel.x;
         mouse_scroll_.y = event.wheel.y;
         break;
      case SDL_QUIT:
         // TODO Should do something for this
         break;
      default:
         break;
      }
   }
}

auto InputManager::GetMouseDown(const short button) -> bool
{
   if (button < 0 || button >= MouseClickTypeCount)
      return false;
   return (is_mouse_down_[button]);
}

bool InputManager::GetMouseUp(const short button)
{
   if (button < 0 || button >= MouseClickTypeCount)
      return false;
   return (is_mouse_up_[button]);
}

bool InputManager::GetMouseButton(const short button) const
{
   switch (button)
   {
   case MouseLeft:
      if (mouse_ & SDL_BUTTON(1))
         return true;
      break;
   case MouseMiddle:
      if (mouse_ & SDL_BUTTON(2))
         return true;
      break;
   case MouseRight:
      if (mouse_ & SDL_BUTTON(3))
         return true;
      break;
   default:
      return false;
   }
   return false;
}

bool InputManager::GetKeyDown(KeyCode key_code)
{
   const int key = static_cast<int>(key_code);
   if (key < 0 || key >= SCANCODE_MAXSIZE)
      return false;
   return (is_key_down_[key]);
}

bool InputManager::GetKeyUp(KeyCode key_code)
{
   const int key = static_cast<int>(key_code);
   if (key < 0 || key >= SCANCODE_MAXSIZE)
      return false;
   return (is_key_up_[key]);
}

bool InputManager::GetKeyButton(KeyCode key_code) const
{
   if (!(keyboard_))
      return false;
   if (keyboard_[static_cast<int>(key_code)])
      return true;
   return false;
}
