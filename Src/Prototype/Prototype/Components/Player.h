#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

namespace godot
{
	//TODO: show fields of godot-related components in editor
	//TODO: create somekind of godot-entity view, to show which components are assigned to entity
	//TODO: move all godot related components to separate folder/filter and create specific naming convention for it
	class Player : public KinematicBody
	{
		GODOT_CLASS(Player, KinematicBody)
	public:
		static void _register_methods() {}
		void _init() {}
	};
}