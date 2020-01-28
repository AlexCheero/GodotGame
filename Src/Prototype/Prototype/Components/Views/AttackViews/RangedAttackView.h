#pragma once

#include <core/Godot.hpp>

#include "../ComponentView.h"
#include "../../AttackComponents.h"

namespace godot
{
	class RangedAttackView : public ComponentView
	{
		GODOT_CLASS(RangedAttackView, ComponentView)
	private:
		int ammoCount;
		float distance;
		float damage;
		float angle;
		float attackTime;
	public:
		static void _register_methods();
		void _init() {}
		RangedAttackComponent GetECSComponent() { return { ammoCount, distance, damage, attackTime, -utils::SecondsToMillis(attackTime) }; }
	};
}