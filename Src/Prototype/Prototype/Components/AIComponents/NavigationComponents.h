#pragma once

#include <PoolArrays.hpp>

struct NavPathComponent
{
	godot::PoolVector3Array path;
	int pathIndex = 0;

	bool PathComplete() { return pathIndex >= path.size(); }
	godot::Vector3 CurrentPathPoint() { return path[pathIndex]; }
};

struct NavAgentComponent
{
	//TODO: why 1-unit height capsule have such origin's y when on_floor checking?
	float agentOriginHeight;
	float agentRadius;
	float minDistance;
};