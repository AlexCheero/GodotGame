#pragma once

#include <core/Godot.hpp>

#include <vector>

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