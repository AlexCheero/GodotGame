#pragma once

#include <core/Godot.hpp>

#include "entt/entt.hpp"

struct VelocityComponent { godot::Vector3 velocity; };
struct SpeedComponent { float speed; };
struct JumpSpeedComponent { float speed; };
struct RotationDirectionComponent { godot::Vector3 direction; };

//<Tags
constexpr entt::hashed_string PlayerTag = "PlayerTag"_hs;
constexpr entt::hashed_string BotTag = "BotTag"_hs;
constexpr entt::hashed_string DeadTag = "DeadTag"_hs;
constexpr entt::hashed_string MainCameraTag = "MainCameraTag"_hs;
//Tags>

constexpr entt::exclude_t ExcludeDead = entt::exclude<entt::tag<DeadTag> >;

struct GravityComponent
{
	float accUp;
	float accDown;
};

//TODO: make debug/test change cam position functional
struct SimpleFollowComponent
{
	float distance;
	float xAngle;
	float yAngle;
	entt::entity targetEntity;
};

struct BoundsComponent
{
	float width;
	float height;
	float length;
	float margin;
};

struct HealthComponent
{
	float hp;
	float maxHp;

	float ProportionOfMax() { return hp / maxHp; }
};