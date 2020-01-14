#pragma once

#include <core/Godot.hpp>

#include "entt/entt.hpp"

namespace godot
{
	struct VelocityComponent { Vector3 velocity; };
	struct SpeedComponent { float speed; };
	struct JumpSpeedComponent { float speed; };
	struct HealthComponent { float hp; };
	struct RotationDirectionComponent { Vector3 direction; };
	
	//<Tags
	constexpr entt::hashed_string PlayerTag = "PlayerTag"_hs;
	constexpr entt::hashed_string DeadTag = "DeadTag"_hs;
	constexpr entt::hashed_string PendingDeleteTag = "PendingDeleteTag"_hs;
	//Tags>

	//TODO: use it everywhere where it fits
	constexpr entt::exclude_t ExcludeDead = entt::exclude<entt::tag<DeadTag>, entt::tag<PendingDeleteTag> >;
	
	struct GravityComponent
	{
		float accUp;
		float accDown;
	};

	//TODO: make debug/test change cam position functional
	struct SimpleFollowComponent
	{
		entt::entity targetEntity;
		float distance;
		float xAngle;
		float yAngle;
	};

	struct BoundsComponent
	{
		float width;
		float height;
		float length;
		float margin;
	};
}