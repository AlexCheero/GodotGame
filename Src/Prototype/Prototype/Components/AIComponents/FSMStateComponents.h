#pragma once

#include "entt/entt.hpp"

//TODO: implement decision making system and don't spread tags all around the codebase
constexpr entt::hashed_string PatrollingTag = "PatrollingTag"_hs;
constexpr entt::hashed_string PathFinishedTag = "PathFinishedTag"_hs;
constexpr entt::hashed_string FleeingTag = "FleeingTag"_hs;

struct PursuingComponent
{
	entt::entity target = entt::null;
	godot::Vector3 previousTargetPosition;
	//TODO: move to view and remove hardcode
	int64_t targetLostMsec;
	float lostInSeconds = 5.f;
};
