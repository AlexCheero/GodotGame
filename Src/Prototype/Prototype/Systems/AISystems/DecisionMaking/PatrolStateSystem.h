#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class PatrolStateSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
