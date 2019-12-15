#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

namespace godot
{
	//TODO: try to made this component-convertible
	class Enemy : public KinematicBody
	{
		GODOT_CLASS(Enemy, KinematicBody)
	private:
		//TODO: think about more accurate way to get enemy health components in AttackSystem
		entt::entity entity;
	public:
		static void _register_methods() {}
		void _init() {}
		entt::entity GetEntity() { return entity; }
		void SetEntity(entt::entity value) { entity = value; }
	};
}