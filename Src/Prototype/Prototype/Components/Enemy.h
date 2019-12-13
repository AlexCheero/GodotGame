#pragma once

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

namespace godot
{
	//TODO: try to made this component-convertible
	class Enemy : public KinematicBody
	{
		GODOT_CLASS(Enemy, KinematicBody)
	public:
		static void _register_methods() {}
		void _init() {}
	};
}