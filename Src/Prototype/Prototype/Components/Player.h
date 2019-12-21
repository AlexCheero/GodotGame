#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

namespace godot
{
	//TODO: show fields of godot-related components in editor
	//TODO: create somekind of godot-entity view, to show which components are assigned to entity
	class Player : public KinematicBody
	{
		GODOT_CLASS(Player, KinematicBody)
	public:
		static void _register_methods() {}
		void _init() {}
	};
}