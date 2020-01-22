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
constexpr entt::hashed_string PendingDeleteTag = "PendingDeleteTag"_hs;
constexpr entt::hashed_string MainCameraTag = "MainCameraTag"_hs;
//Tags>

//TODO: ask skypjack can I add excludes?
//TODO: it probably even doesn' needed
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

struct HealthComponent
{
	float hp;
	float maxHp;

	float ProportionOfMax() { return hp / maxHp; }
};