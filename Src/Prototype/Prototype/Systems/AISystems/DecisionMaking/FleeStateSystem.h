#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class FleeStateSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
