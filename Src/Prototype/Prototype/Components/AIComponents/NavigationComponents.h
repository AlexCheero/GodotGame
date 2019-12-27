#pragma once

#include <PoolArrays.hpp>

struct NavPathComponent
{
	godot::PoolVector3Array path;
};

struct NavAgentComponent
{
	//TODO: why 1-unit height capsule have such origin's y when on_floor checking?
	float agentOriginHeight = 1.742f;
	float agentRadius = 1.f;
	float minDistance = 0.1f;
	int pathIndex = 1;
	int prevPathIndex = -1;
};