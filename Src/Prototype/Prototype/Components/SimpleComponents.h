#pragma once

#include <core/Godot.hpp>

#include "entt/entt.hpp"

#include "ComponentsMeta.h"

constexpr entt::hashed_string PlayerTag = "PlayerTag"_hs;
constexpr entt::hashed_string BotTag = "BotTag"_hs;
constexpr entt::hashed_string DeadTag = "DeadTag"_hs;
constexpr entt::hashed_string MainCameraTag = "MainCameraTag"_hs;

struct VelocityComponent { godot::Vector3 velocity; };
struct SpeedComponent { float speed; }; REGISTER_COMPONENT(SpeedComponent, "speed");
struct JumpSpeedComponent { float speed; }; REGISTER_COMPONENT(JumpSpeedComponent, "speed");
struct RotationDirectionComponent { godot::Vector3 direction; };

constexpr entt::exclude_t ExcludeDead = entt::exclude<entt::tag<DeadTag> >;

struct GravityComponent
{
	float accUp;
	float accDown;
};
REGISTER_COMPONENT(GravityComponent, "acceleration Up", "acceleration Down");

struct SimpleFollowComponent
{
	float distance;
	float xAngle;
	float yAngle;
	entt::entity targetEntity;
};
REGISTER_COMPONENT(SimpleFollowComponent, "distance", "xAngle", "yAngle");

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
	float criticalProportion;

	bool IsHealthCritical() { return hp / maxHp <= criticalProportion; }
};
REGISTER_COMPONENT(HealthComponent, "hp", "maxHp", "criticalProportion");

struct StunComponent
{
	float secondsLeft;
	float speedFactor;
};