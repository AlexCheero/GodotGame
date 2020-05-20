#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class RangedAttackSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}