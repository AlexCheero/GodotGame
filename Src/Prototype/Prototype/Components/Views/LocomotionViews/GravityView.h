#pragma once

#include <core/Godot.hpp>

#include "../ComponentView.h"
#include "../../SimpleComponents.h"

namespace godot
{
	class GravityView : public ComponentView
	{
		GODOT_CLASS(GravityView, ComponentView)
	private:
		float accUp;
		float accDown;
	public:
		static void _register_methods();
		void _init() {}
		GravityComponent GetECSComponent() { return { accUp, accDown }; }
	};
}