#pragma once
#include <SDL.h>

class Camera;
class InputHandler;

class GameObject
{
   //TODO Make a physics Object?
public:
   virtual ~GameObject() = default;
   SDL_Rect transform{};

   virtual void Start()
   {
   };

   virtual void Update()
   {
   };

   virtual void FixedUpdate()
   {
      
   };

   virtual void Destroy()
   {
   };

   //x TODO this could probably be made into its own interface?
   //x  virtual void SubscribeInputs(InputHandler* inputHandler) {};
   //x  virtual void UpdateInput(const int eventType, const SDL_Event* _event) { };

   //TODO Move this into a renderer class?
   // TODO Hopefully temp draw method?
   virtual bool Draw(Camera* camera) { return false; };
};
