#pragma once

#include <InputEvent.hpp>
#include <Input.hpp>

#include "../../Components/SimpleComponents.h"

namespace godot
{
	class PlayerInputSystem
	{
	private:
		template<typename T>
		static void ProcessInputKey(entt::registry& registry, bool pressed);

		template<typename T>
		static void ProcessJustPressedKey(entt::registry& registry, Input* pInput, String action);

		template<typename T>
		static void ProcessInputAxis(entt::registry& registry, Vector2 direction);

		static Vector2 GetInputDirection(Input* pInput, String actionPrefix);
	public:
		static void HandleInput(entt::registry& registry, InputEvent* e);
	};
}
