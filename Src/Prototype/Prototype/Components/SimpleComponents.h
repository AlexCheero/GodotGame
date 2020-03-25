#pragma once

#include <core/Godot.hpp>

#include "entt/entt.hpp"

//TODO: probably set velocity for bot in single system, like PlayerVelocitySystem (maybe even merge with it)
//		by input and set only input for bot in decision making system
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

//TODO: ask skypjack can I append excludes?
//TODO: change all exclude DeadTag to ExcludeDead or remove it
constexpr entt::exclude_t ExcludeDead = entt::exclude<entt::tag<DeadTag> >;

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

struct HealthComponent
{
	float hp;
	float maxHp;

	float ProportionOfMax() { return hp / maxHp; }
};