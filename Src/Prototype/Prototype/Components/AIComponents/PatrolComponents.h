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
	constexpr static int PROPERTIES_COUNT = 5;

	float viewAngleSmall;
	float viewAngleBig;
	float longViewDistance;
	float shortViewDistance;
	//TODO: probably this field doesn't fit here
	float looseTargetTime;
};