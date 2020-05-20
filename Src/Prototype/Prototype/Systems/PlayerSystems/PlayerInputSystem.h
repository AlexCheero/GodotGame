#pragma once

#include <InputEvent.hpp>

#include "../../Components/SimpleComponents.h"

namespace godot
{
	namespace PlayerInputSystem
	{
		void HandleInput(entt::registry& registry/*, InputEvent* e*/);
	}
}
