#pragma once

#include <PoolArrays.hpp>

#include <entt/entt.hpp>

#include "../ComponentsMeta.h"

struct NavPathComponent
{
	godot::PoolVector3Array path;
	int pathIndex = 0;

	bool PathComplete() { return pathIndex >= path.size(); }
	godot::Vector3 CurrentPathPoint() { return path[pathIndex]; }
};

REGISTRABLE_COMPONENT(NavMarginComponent)
{
	float margin;
};