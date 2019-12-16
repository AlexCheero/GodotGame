#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

namespace godot
{
	class Enemy : public KinematicBody
	{
		GODOT_CLASS(Enemy, KinematicBody)
	private:
		entt::entity entity;
	public:
		static void _register_methods() {}
		void _init() {}
		entt::entity GetEntity() { return entity; }
		void SetEntity(entt::entity value) { entity = value; }
	};
}