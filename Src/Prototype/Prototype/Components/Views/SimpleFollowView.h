#pragma once

#include <core/Godot.hpp>

#include "ComponentView.h"
#include "../SimpleComponents.h"

namespace godot
{
	class SimpleFollowView : public ComponentView
	{
		GODOT_CLASS(SimpleFollowView, ComponentView)
	private:
		float distance;
		float xAngle;
		float yAngle;
	public:
		static void _register_methods();
		void _init() {}
		SimpleFollowComponent GetECSComponent() { return { distance, xAngle, yAngle }; }
	};
}