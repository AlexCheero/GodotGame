#pragma once

#include <core/Godot.hpp>

namespace godot
{
	struct VelocityComponent { Vector3 velocity; };
	struct RotationComponent { };
	struct SpeedComponent { float speed; };
	struct JumpSpeedComponent { float speed; };
	struct HealthComponent { float hp; };
	struct DeadComponent { };
	struct PendingDeleteComponent { };
	
	struct GravityComponent
	{
		float accUp;
		float accDown;
	};

	struct CamPositionComponent
	{
		float distance;
		float xAngle;
		float yAngle;
	};

	struct AttackComponent
	{
		float distance;
		float damage;
		float angle;
	};
}