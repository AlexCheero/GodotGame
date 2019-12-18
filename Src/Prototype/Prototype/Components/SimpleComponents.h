#pragma once

#include <core/Godot.hpp>

namespace godot
{
	struct VelocityComponent { Vector3 velocity; };
	struct SpeedComponent { float speed; };
	struct JumpSpeedComponent { float speed; };
	struct HealthComponent { float hp; };
	
	//<Tags
	struct RotationTag { };
	struct DeadTag { };
	struct PendingDeleteTag { };
	//Tags>

	//<Input components
	struct PlayerInputTag { };
	struct AttackedInputTag { };
	//Input components>
	
	struct GravityComponent
	{
		float accUp;
		float accDown;
	};

	struct SimpleFollowComponent
	{
		float distance;
		float xAngle;
		float yAngle;
	};
}