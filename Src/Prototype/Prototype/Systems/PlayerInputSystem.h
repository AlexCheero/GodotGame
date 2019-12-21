#pragma once

#include "BaseSystem.h"

#include <Input.hpp>

namespace godot
{
	class PlayerInputSystem : public BaseSystem
	{
		static Vector2 GetInputDirection(Input* pInput, const char* actionPrefix);
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
