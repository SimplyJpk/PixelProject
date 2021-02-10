#include "FlyCamera.h"

FlyCamera::FlyCamera()
{
}

// Updates camera position and rotation
void FlyCamera::Update(const float delta)
{
   if (InputManager::Instance()->GetKeyButton(KeyCode::Space))
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

	if (im->GetKeyButton(KeyCode::LShift))
		move_speed_ = default_speed_ * shift_multi_;
	else
		move_speed_ = default_speed_;

	// Forward Back
	if (im->GetKeyButton(KeyCode::W))
	{
		currentPos -= forward* delta* move_speed_;
	}
	else if (im->GetKeyButton(KeyCode::S))
	{
		currentPos += forward* delta * move_speed_;
	}
	// Left Right
	if (im->GetKeyButton(KeyCode::A))
	{
		currentPos -= left* delta* move_speed_;
	}
	else if (im->GetKeyButton(KeyCode::D))
	{
		currentPos += left* delta * move_speed_;
	}
	// Up Down
	if (im->GetKeyButton(KeyCode::Q))
	{
		currentPos -= up* delta* move_speed_;
	}
	else if (im->GetKeyButton(KeyCode::E))
	{
		currentPos += up* delta * move_speed_;
	}

	SetPosition(currentPos);
}