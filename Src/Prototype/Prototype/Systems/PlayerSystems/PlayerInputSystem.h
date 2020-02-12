#pragma once

#include "../BaseSystem.h"

#include <InputEvent.hpp>

namespace godot
{
	//TODO: change name not to confuse it with classes that are inherited from base system
	class PlayerInputSystem
	{
		static Vector2 GetInputDirection(const char* actionPrefix);
	public:
		void operator()(entt::registry& registry, InputEvent* e);
	};
}
