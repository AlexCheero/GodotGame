#pragma once

#include <core/Godot.hpp>

#include "entt/entt.hpp"

#include "ComponentsMeta.h"

DECLARE_REGISTERED_TAG(PlayerTag);
DECLARE_REGISTERED_TAG(BotTag);
DECLARE_REGISTERED_TAG(MainCameraTag);

DECLARE_TAG(DeadTag);
DECLARE_TAG(InAirTag);

//TODO: remove or use everywhere
constexpr entt::exclude_t ExcludeDead = entt::exclude<DeadTag>;

struct VelocityComponent { godot::Vector3 velocity; };
struct JumpSpeedComponent { float speed; }; REGISTER_COMPONENT(JumpSpeedComponent, "speed");
struct RotationDirectionComponent { godot::Vector3 direction; };

struct SpeedComponent
{
	float speed;
	float dashSpeed;
};
REGISTER_COMPONENT(SpeedComponent, "speed", "dashSpeed");

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
};
REGISTER_COMPONENT(HealthComponent, "hp", "maxHp");