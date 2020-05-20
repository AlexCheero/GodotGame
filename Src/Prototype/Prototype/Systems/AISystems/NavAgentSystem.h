#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class NavAgentSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
