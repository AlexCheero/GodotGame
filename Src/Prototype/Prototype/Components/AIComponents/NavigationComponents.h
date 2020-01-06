#pragma once

#include <PoolArrays.hpp>

#include <vector>

struct NavPathComponent
{
	godot::PoolVector3Array path;
	int pathIndex = 0;

	//TODO: probably should move all methods from components to systems
	bool PathComplete() { return pathIndex >= path.size(); }
	godot::Vector3 CurrentPathPoint() { return path[pathIndex]; }
};

struct NavAgentComponent
{
	float agentOriginHeight;
	float agentRadius;
	float minDistance;
};

struct PatrolRoute
{
	std::vector<godot::Vector3> routePoints;
	int current = 0;

	godot::Vector3 GetCurrentPatrolPoint() { return routePoints[current]; }
};