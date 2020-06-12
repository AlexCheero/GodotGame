#pragma once

#include "entt/entt.hpp"

#include <Vector2.hpp>

namespace godot
{
	class PlayerAttackInputSystem
	{
	private:
		static float ClampInputAngle(Vector2 dir);
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
