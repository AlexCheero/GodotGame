#pragma once

#include "../BaseSystem.h"

#include <InputEvent.hpp>

namespace godot
{
	//TODO1: change name not to confuse it with classes that are inherited from base system
	class PlayerInputSystem
	{
		//TODO1: come up with what to do with static methods
		static void GetInputDirection(Vector2& dir, InputEvent* e, const char* actionPrefix);
	public:
		void operator()(entt::registry& registry, InputEvent* e);
	};
}
