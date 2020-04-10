#pragma once

#include <PoolArrays.hpp>

#include <entt/entt.hpp>

struct NavPathComponent
{
	godot::PoolVector3Array path;
	int pathIndex = 0;

	bool PathComplete() { return pathIndex >= path.size(); }
	godot::Vector3 CurrentPathPoint() { return path[pathIndex]; }
};

struct NavMarginComponent
{
	constexpr static int PROPERTIES_COUNT = 1;

	float margin;
};