#pragma once

#include <core/Godot.hpp>
#include <Area.hpp>
#include <KinematicBody.hpp>

#include "entt/entt.hpp"

namespace godot
{
	//TODO0: take collision layer from MeleeAttackComponent if it even needed
	class HTHDamagingArea : public Area
	{
		GODOT_CLASS(HTHDamagingArea, Area)
	private:
		entt::entity ownerEntity;

		void _on_Area_body_entered(KinematicBody* pBody);
		void _assign_owner_entity();
	public:
		static void _register_methods();
		void _init() {}
		void _ready();
	};
}
