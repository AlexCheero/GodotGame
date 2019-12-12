#pragma once

#include <core/Godot.hpp>

namespace godot
{
	struct VelocityComponent { Vector3 velocity; };
	struct RotationComponent { };
	struct SpeedComponent { float speed; };
	struct JumpSpeedComponent { float speed; };
	
	struct GravityComponent
	{
		float accUp;
		float accDown;
	};

	struct CamRelativePositionComponent
	{
		float distance;
		float xAngle;
		float yAngle;
	};
}