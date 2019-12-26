#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

#include "entt/entt.hpp"

//includes for NavigationComponent
#include <PoolArrays.hpp>
//------------------------------->

namespace godot
{
	//TODO: move to separate file
	struct NavigationComponent
	{
		float moveSpeed; //TODO: use SpeedComponent instead
		PoolVector3Array path;
		int pathIndex = 0;
	};

	//TODO: check godot-view components for bloating
	class Enemy : public KinematicBody
	{
		GODOT_CLASS(Enemy, KinematicBody)
	private:
		entt::entity entity;
	public:
		entt::entity GetEntity() const { return entity; }
		void SetEntity(entt::entity value) { entity = value; }

		//TODO: create some kind of followingPathComponent instead of fields in Enemy
		//this TODO doubles TODO from NavAgentSystem.cpp
		//Vector3 moveTarget;
		NavigationComponent navigation;

		static void _register_methods() {}
		//TODO: check that none of godot-view components (maybe check all the other components and systems)
		//initializes stuff inside. all the initialization must take place in ECSWorld!
		void _init() {}
	};
}