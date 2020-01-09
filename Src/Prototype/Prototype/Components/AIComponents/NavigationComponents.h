#pragma once

#include <PoolArrays.hpp>

#include <entt/entt.hpp>

//TODO: move all components into godot namespace or remove SimpleComponents from it
struct NavPathComponent
{
	godot::PoolVector3Array path;
	int pathIndex = 0;

	//TODO: probably should move all methods from components to systems
	bool PathComplete() { return pathIndex >= path.size(); }
	godot::Vector3 CurrentPathPoint() { return path[pathIndex]; }
};

struct NavMarginComponent
{
	float margin;
};

//TODO: probably move to something like FsmStateTags.h
constexpr entt::hashed_string PathFinishedTag = "PathFinishedTag"_hs;