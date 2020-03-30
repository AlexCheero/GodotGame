#pragma once

#include <core/CoreTypes.hpp>

#include "entt/entt.hpp"

struct PursuingStateComponent
{
	entt::entity target = entt::null;
	godot::Vector3 previousTargetPosition;
	int64_t targetLostMsec = -1;
};

struct HittedFromComponent
{
	godot::Vector3 position;
};

struct PatrolLookAroundComponent
{
	//TODO: remove hardcode
	float time = 1;
};

constexpr entt::hashed_string PatrolStateTag = "PatrolStateTag"_hs;
constexpr entt::hashed_string MeleeAttackStateTag = "MeleeAttackStateTag"_hs;
constexpr entt::hashed_string FleeStateTag = "FleeStateTag"_hs;

