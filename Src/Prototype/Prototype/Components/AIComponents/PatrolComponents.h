#pragma once

#include <core/Godot.hpp>

#include <vector>

#include "entt/entt.hpp"

struct PatrolRouteComponent
{
	std::vector<godot::Vector3> routePoints;
	int current = 0;

	godot::Vector3 GetCurrentPatrolPoint() { return routePoints[current]; }
};

struct PatrolmanComponent
{
	float viewAngleSmall;
	float viewAngleBig;
	float longViewDistance;
	float shortViewDistance;
};

struct PursuingComponent
{
	entt::entity target = entt::null;
	godot::Vector3 previousTargetPosition;
};

//TODO: probably move to something like FsmStateTags.h
constexpr entt::hashed_string PatrollingTag = "PatrollingTag"_hs;