#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class MeleeAttackCooldownSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
