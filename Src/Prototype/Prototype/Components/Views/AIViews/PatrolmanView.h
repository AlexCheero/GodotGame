#pragma once

#include <core/Godot.hpp>

#include "../ComponentView.h"
#include "../../AIComponents/PatrolComponents.h"

namespace godot
{
	class PatrolmanView : public ComponentView
	{
		GODOT_CLASS(PatrolmanView, ComponentView)
	private:
		float viewAngleSmall;
		float viewAngleBig;
		float longViewDistance;
		float shortViewDistance;
		float looseTargetTime;
	public:
		static void _register_methods();
		void _init() {}
		PatrolmanComponent GetECSComponent() { return { viewAngleSmall, viewAngleBig, longViewDistance, shortViewDistance, looseTargetTime }; }
	};
}