#pragma once

#include <core/Godot.hpp>

#include "entt/entt.hpp"

#include "ComponentsMeta.h"

struct VelocityComponent { godot::Vector3 velocity; };
struct SpeedComponent { float speed; }; REGISTER_COMPONENT(SpeedComponent, 1);
struct JumpSpeedComponent { float speed; }; REGISTER_COMPONENT(JumpSpeedComponent, 1);
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
REGISTER_COMPONENT(GravityComponent, 2);

struct SimpleFollowComponent
{
	float distance;
	float xAngle;
	float yAngle;
	entt::entity targetEntity;
};
REGISTER_COMPONENT(SimpleFollowComponent, 3);

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
REGISTER_COMPONENT(HealthComponent, 2);