#pragma once

#include <core/Godot.hpp>

#include "entt/entt.hpp"

#include "ComponentsMeta.h"

DECLARE_TAG(PlayerTag);
DECLARE_TAG(BotTag);
DECLARE_TAG(DeadTag);
DECLARE_TAG(MainCameraTag);

struct VelocityComponent { godot::Vector3 velocity; };
struct SpeedComponent { float speed; }; REGISTER_COMPONENT(SpeedComponent, "speed");
struct JumpSpeedComponent { float speed; }; REGISTER_COMPONENT(JumpSpeedComponent, "speed");
struct RotationDirectionComponent { godot::Vector3 direction; };

constexpr entt::exclude_t ExcludeDead = entt::exclude<DeadTag>;

struct GravityComponent
{
	float accUp;
	float accDown;
};
REGISTER_COMPONENT(GravityComponent, "accelerationUp", "accelerationDown");

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

	//TODO: maybe this should be in some kind of decision making view
	bool IsHealthCritical() { return hp / maxHp <= criticalProportion; }
};
REGISTER_COMPONENT(HealthComponent, "hp", "maxHp", "criticalProportion");

struct StunComponent
{
	float secondsLeft;
	float speedFactor;
};