#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class FleeingSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
