#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class ClearBotInputSystem
	{
	private:
		template<typename Type, typename... Types>
		static void ClearInput(entt::registry& registry);
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}