#pragma once

#include <core/Godot.hpp>

#include <vector>

#include "entt/entt.hpp"

#include "../ComponentsMeta.h"

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
	float looseTargetTime;
	float lookAroundTime;
};
REGISTER_COMPONENT(PatrolmanComponent, "viewAngleSmall", "viewAngleBig", "longViewDistance", "shortViewDistance", "looseTargetTime", "lookAroundTime");