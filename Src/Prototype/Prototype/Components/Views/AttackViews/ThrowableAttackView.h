#pragma once

#include <core/Godot.hpp>

#include "../ComponentView.h"
#include "../../AttackComponents.h"

namespace godot
{
	class ThrowableAttackView : public ComponentView
	{
		GODOT_CLASS(ThrowableAttackView, ComponentView)
	private:
		int ammoCount;
		godot::Ref<godot::PackedScene> throwableScene;
		float force;
		float attackTime;
	public:
		static void _register_methods();
		void _init() {}
		ThrowableAttackComponent GetECSComponent() { return { ammoCount, throwableScene, force, attackTime, -utils::SecondsToMillis(attackTime) }; }
	};
}