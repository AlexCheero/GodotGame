#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

namespace godot
{
	//TODO: try to made this component-convertible
	class Player : public KinematicBody
	{
		GODOT_CLASS(Player, KinematicBody)
	public:
		static void _register_methods() {}
		void _init() {}
	};
}