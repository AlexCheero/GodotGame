#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class PursueStateSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
