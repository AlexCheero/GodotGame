#pragma once

#include "../BaseSystem.h"

#include <InputEvent.hpp>

#include "../../Components/SimpleComponents.h"

namespace godot
{
	class PlayerInputSystem
	{
	private:
		Vector2 GetInputDirection(const char* actionPrefix);

		template<typename T>
		void ProcessInput(entt::registry& registry, InputEvent* e, bool pressed);
	public:
		void operator()(entt::registry& registry, InputEvent* e);
	};

	template<typename T>
	inline void PlayerInputSystem::ProcessInput(entt::registry& registry, InputEvent* e, bool pressed)
	{
		if (pressed)
		{
			auto view = registry.view<PlayerTag>(entt::exclude<T>);
			registry.assign<T>(view.begin(), view.end());
		}
		else
		{
			auto view = registry.view<PlayerTag, T>();
			registry.remove<T>(view.begin(), view.end());
		}
	}
}
