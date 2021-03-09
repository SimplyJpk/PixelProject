#include "FlyCamera.h"

FlyCamera::FlyCamera()
{
}

// Updates camera position and rotation
void FlyCamera::Update(const float delta)
{
   if (InputManager::Instance()->IsMovementKeysDown())
   {
      MoveCam(delta);
   }
}

// Move Camera based on inputs
void FlyCamera::MoveCam(const float delta)
{
   auto currentPos = GetPosition();
   const auto left = Row(0);
   const auto up = Row(1);
   const auto forward = Row(2);

  auto* im = InputManager::Instance();

	// Left Right
	if (im->IsKeyHeld(KeyCode::A))
	{
		currentPos += left* delta* move_speed_;
	}
	else if (im->IsKeyHeld(KeyCode::D))
	{
		currentPos -= left* delta * move_speed_;
	}
	// Up Down
	if (im->IsKeyHeld(KeyCode::S))
	{
		currentPos -= up* delta* move_speed_;
	}
	else if (im->IsKeyHeld(KeyCode::W))
	{
		currentPos += up* delta * move_speed_;
	}
	 	// Forward Back
	if (im->IsKeyHeld(KeyCode::Q))
	{
		currentPos -= forward* delta* move_speed_;
	}
	else if (im->IsKeyHeld(KeyCode::E))
	{
		currentPos += forward* delta * move_speed_;
	}

	SetPosition(currentPos);
}