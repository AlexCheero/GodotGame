#pragma once

#include <core/Godot.hpp>
#include <RigidBody.hpp>

namespace godot
{
	class Throwable : public RigidBody
	{
		GODOT_CLASS(Throwable, RigidBody)
	public:
		static void _register_methods();
		void _init();
		void _on_throwable_collide(Object* body);
	};
}