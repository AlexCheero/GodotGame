#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class CheckForPileInSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}

