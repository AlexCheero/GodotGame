#pragma once

#include <InputEvent.hpp>

#include "../../Components/SimpleComponents.h"

namespace godot
{
	//TODO0: turn into class
	namespace PlayerInputSystem
	{
		void HandleInput(entt::registry& registry/*, InputEvent* e*/);
	}
}
