#pragma once

#include <core/Godot.hpp>

#include "../Utils.h"

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

	//TODO: move this component and #include "../Utils.h" to separate file
	struct AttackComponent
	{
		float distance;
		float damage;
		float angle;
		float attackTime;
		//int64_t prevHitTime = static_cast<int64_t>(-static_cast<double>(attackTime) * 1000);
		int64_t prevHitTime = -utils::SecondsToMillis(attackTime);
	};
}