#pragma once

#include <core/Godot.hpp>

#include "../ComponentView.h"
#include "../../AttackComponents.h"

namespace godot
{
	class MeleeAttackView : public ComponentView
	{
		GODOT_CLASS(MeleeAttackView, ComponentView)
	private:
		float distance;
		float angle;
		float attackTime;
		//TODO: make it int for all views
		String collisionLayerName;
	public:
		static void _register_methods();
		void _init() {}
	};
}