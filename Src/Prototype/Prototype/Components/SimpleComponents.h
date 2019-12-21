#pragma once

#include <core/Godot.hpp>

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

	//<Input components
	constexpr entt::hashed_string PlayerInputTag = "PlayerInputTag"_hs;

	//TODO: move this components into InputComponent as fields
	struct InputRotationComponent { Vector2 dir; };
	struct InputVelocityComponent { Vector2 dir; };

	//TODO: make input bitmask
	struct InputComponent
	{
		bool attack;
		bool jump;
	};

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