#pragma once

#include <core/Godot.hpp>

#include "entt/entt.hpp"

#include "ComponentsMeta.h"

DECLARE_REGISTERED_TAG(PlayerTag);
DECLARE_REGISTERED_TAG(BotTag);
DECLARE_REGISTERED_TAG(MainCameraTag);

TAG(DeadTag);
TAG(InAirTag);

constexpr entt::exclude_t ExcludeDead = entt::exclude<DeadTag>;

struct VelocityComponent { godot::Vector3 velocity; };
REGISTRABLE_COMPONENT(JumpSpeedComponent) { float speed; };
struct RotationDirectionComponent { godot::Vector3 direction; };

REGISTRABLE_COMPONENT(SpeedComponent)
{
	float speed;
	float dashSpeed;
};

REGISTRABLE_COMPONENT(GravityComponent)
{
	float accelerationUp;
	float accelerationDown;
};

struct BoundsComponent
{
	float width;
	float height;
	float length;
	float margin;
};

REGISTRABLE_COMPONENT(HealthComponent)
{
	float hp;
	float maxHp;
};