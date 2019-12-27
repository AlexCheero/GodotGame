#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

#include "entt/entt.hpp"

namespace godot
{
	//TODO: check godot-view components for bloating
	class Enemy : public KinematicBody
	{
		GODOT_CLASS(Enemy, KinematicBody)
	private:
		entt::entity entity;
	public:
		entt::entity GetEntity() const { return entity; }
		void SetEntity(entt::entity value) { entity = value; }

		static void _register_methods() {}
		//TODO: check that none of godot-view components (maybe check all the other components and systems)
		//initializes stuff inside. all the initialization must take place in ECSWorld!
		void _init() {}
	};
}