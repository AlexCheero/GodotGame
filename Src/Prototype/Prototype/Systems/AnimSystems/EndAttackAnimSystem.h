#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class EndAttackAnimSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
