#pragma once

#include <core/CoreTypes.hpp>

#include "entt/entt.hpp"

//TODO: implement decision making system and don't spread tags all around the codebase
constexpr entt::hashed_string FleeingTag = "FleeingTag"_hs;

struct PursuingStateComponent
{
	entt::entity target = entt::null;
	godot::Vector3 previousTargetPosition;
	int64_t targetLostMsec;
};

constexpr entt::hashed_string PatrolStateTag = "PatrolStateTag"_hs;
constexpr entt::hashed_string FleeStateTag = "FleeStateTag"_hs;
constexpr entt::hashed_string AttackStateTag = "AttackStateTag"_hs;
