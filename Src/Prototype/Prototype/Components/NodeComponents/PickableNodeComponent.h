#pragma once

#include <core/Godot.hpp>
#include <Area.hpp>

namespace godot
{
	class PickableNodeComponent : public Area
	{
		GODOT_CLASS(PickableNodeComponent, Area)
	public:
		static void _register_methods();
		void _init() {}
		void _on_Pickable_body_entered(Object* body);
	};
}
