#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class MeleeStateSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
