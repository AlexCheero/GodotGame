#pragma once

#include <core/Godot.hpp>

#include "ComponentView.h"
#include "../SimpleComponents.h"

namespace godot
{
	class HealthView : public ComponentView
	{
		GODOT_CLASS(HealthView, ComponentView)
	private:
		float hp;
		float maxHp;
	public:
		static void _register_methods();
		void _init() {}
		HealthComponent GetECSComponent() { return { hp, maxHp }; }
	};
}