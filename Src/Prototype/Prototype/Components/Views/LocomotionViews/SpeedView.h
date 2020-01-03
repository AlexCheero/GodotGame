#pragma once

#include <core/Godot.hpp>

#include "../ComponentView.h"
#include "../../SimpleComponents.h"

namespace godot
{
	class SpeedView : public ComponentView
	{
		GODOT_CLASS(SpeedView, ComponentView)
	private:
		float speed;
	public:
		static void _register_methods();
		void _init() {}
		SpeedComponent GetECSComponent() { return { speed }; }
	};
}