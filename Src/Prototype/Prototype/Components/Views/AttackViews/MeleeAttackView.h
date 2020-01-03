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
		float damage;
		float angle;
		float attackTime;
	public:
		static void _register_methods();
		void _init() {}
		MeleeAttackComponent GetECSComponent() { return { distance, damage, angle, attackTime, -utils::SecondsToMillis(attackTime) }; }
	};
}