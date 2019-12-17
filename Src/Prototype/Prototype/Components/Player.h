#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

namespace godot
{
	//TODO: show fields of godot-related components in editor
	class Player : public KinematicBody
	{
		GODOT_CLASS(Player, KinematicBody)
	public:
		static void _register_methods() {}
		void _init() {}
	};
}