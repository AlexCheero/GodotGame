#pragma once

#include "BaseSystem.h"

#include <Input.hpp>

#include "../Components/SimpleComponents.h"

namespace godot
{
	class PlayerInputSystem : public BaseSystem
	{
		static Vector2 GetInputDirection(Input* pInput, const char* actionPrefix);
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
