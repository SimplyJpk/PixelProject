#pragma once
#include "Camera.h"

#include "InputManager.h"

class FlyCamera : public Camera
{
public:
	FlyCamera();

  void Update(float delta) override;
private:
	// Default speed and Shift Multiplier, holding shift will move camera default*mult faster
	float default_speed_ = 1.0f;
	float shift_multi_ = 5;

	float move_speed_ = 1.0f;

	// Moves the Camera
	void MoveCam(float delta);
};

