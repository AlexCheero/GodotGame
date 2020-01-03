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
		static void _register_methods()
		{
			register_property<MeleeAttackView, float>("distance", &MeleeAttackView::distance, 0);
			register_property<MeleeAttackView, float>("damage", &MeleeAttackView::damage, 0);
			register_property<MeleeAttackView, float>("angle", &MeleeAttackView::angle, 0);
			register_property<MeleeAttackView, float>("attackTime", &MeleeAttackView::attackTime, 0);
		}

		void _init() {}

		MeleeAttackComponent GetECSComponent()
		{
			MeleeAttackComponent component;
			component.distance = distance;
			component.damage = damage;
			component.angle = angle;
			component.attackTime = attackTime;
			component.prevHitTime = -utils::SecondsToMillis(attackTime);

			return component;
		}
	};
}