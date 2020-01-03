#pragma once

#include <core/Godot.hpp>

#include "../ComponentView.h"
#include "../../SimpleComponents.h"

namespace godot
{
	class JumpSpeedView : public ComponentView
	{
		GODOT_CLASS(JumpSpeedView, ComponentView)
	private:
		float speed;
	public:
		static void _register_methods();
		void _init() {}
		JumpSpeedComponent GetECSComponent() { return { speed }; }
	};
}