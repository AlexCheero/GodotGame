#pragma once

#include <core/Godot.hpp>

#include "../ComponentView.h"
#include "../../AttackComponents.h"

namespace godot
{
	class GrenadeView : public ComponentView
	{
		GODOT_CLASS(GrenadeView, ComponentView)
	private:
		float explosionTime;
		float explosionRadius;
		float damage;
	public:
		static void _register_methods();
		void _init() {}
		GrenadeComponent GetECSComponent() { return { explosionTime, explosionRadius, damage }; }
	};
}