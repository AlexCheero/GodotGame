#pragma once

#include <core/Godot.hpp>

#include "../ComponentView.h"
#include "../../AIComponents/NavigationComponents.h"

namespace godot
{
	class NavMarginView : public ComponentView
	{
		GODOT_CLASS(NavMarginView, ComponentView)
	private:
		float margin;
	public:
		static void _register_methods();
		void _init() {}
		NavMarginComponent GetECSComponent() { return { margin }; }
	};
}