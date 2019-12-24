#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

#include "entt/entt.hpp"

namespace godot
{
	class Enemy : public KinematicBody
	{
		GODOT_CLASS(Enemy, KinematicBody)
	private:
		entt::entity entity;
	public:
		entt::entity GetEntity() const { return entity; }
		void SetEntity(entt::entity value) { entity = value; }

		static void _register_methods() {}
		void _init() {}
	};
}