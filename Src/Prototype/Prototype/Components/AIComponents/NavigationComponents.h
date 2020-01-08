#pragma once

#include <PoolArrays.hpp>

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