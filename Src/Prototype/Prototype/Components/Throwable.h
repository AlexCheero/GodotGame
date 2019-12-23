#pragma once

#include <core/Godot.hpp>
#include <RigidBody.hpp>

namespace godot
{
	class Throwable : public RigidBody
	{
		GODOT_CLASS(Throwable, RigidBody)
	public:
		static void _register_methods() { register_method((char*)"_on_Box_body_entered", &Throwable::_on_body_shape_entered); }
		void _init() {}

		void _on_body_shape_entered(Object* body) { Godot::print("Throwable::_on_body_shape_entered"); }
	};
}