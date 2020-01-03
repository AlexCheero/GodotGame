#pragma once

#include <core/Godot.hpp>

#include "entt/entt.hpp"

namespace godot
{
	struct VelocityComponent { Vector3 velocity; };
	struct SpeedComponent { float speed; };
	struct JumpSpeedComponent { float speed; };
	struct HealthComponent { float hp; };
	
	//<Tags
	constexpr entt::hashed_string RotationTag = "RotationTag"_hs;
	constexpr entt::hashed_string DeadTag = "DeadTag"_hs;
	constexpr entt::hashed_string PendingDeleteTag = "PendingDeleteTag"_hs;
	//Tags>
	
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