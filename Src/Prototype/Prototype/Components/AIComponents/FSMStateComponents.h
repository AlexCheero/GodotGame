#pragma once

#include "entt/entt.hpp"

//TODO: implement decision making system and don't spread tags all around the codebase
constexpr entt::hashed_string PatrollingTag = "PatrollingTag"_hs;
constexpr entt::hashed_string PathFinishedTag = "PathFinishedTag"_hs;

struct PursuingComponent
{
	entt::entity target = entt::null;
	godot::Vector3 previousTargetPosition;
};
