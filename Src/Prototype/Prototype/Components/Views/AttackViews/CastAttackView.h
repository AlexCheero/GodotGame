#pragma once

#include <core/Godot.hpp>

#include "../ComponentView.h"
#include "../../AttackComponents.h"

namespace godot
{
	class CastAttackView : public ComponentView
	{
		GODOT_CLASS(CastAttackView, ComponentView)
	private:
		float distance;
		float damage;
		float angle;
		float attackTime;
	public:
		static void _register_methods();
		void _init() {}
		CastAttackComponent GetECSComponent() { return {distance, damage, attackTime, -utils::SecondsToMillis(attackTime) }; }
	};
}