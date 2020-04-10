#pragma once

#include <core/Godot.hpp>

#include "entt/entt.hpp"

struct VelocityComponent { godot::Vector3 velocity; };
struct SpeedComponent { constexpr static int PROPERTIES_COUNT = 1; float speed; };
struct JumpSpeedComponent { constexpr static int PROPERTIES_COUNT = 1; float speed; };
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
	constexpr static int PROPERTIES_COUNT = 2;

	float accUp;
	float accDown;
};

//TODO: make debug/test change cam position functional
struct SimpleFollowComponent
{
	constexpr static int PROPERTIES_COUNT = 3;

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
	constexpr static int PROPERTIES_COUNT = 2;

	float hp;
	float maxHp;

	float ProportionOfMax() { return hp / maxHp; }
};