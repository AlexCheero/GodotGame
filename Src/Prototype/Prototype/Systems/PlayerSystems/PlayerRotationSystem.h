#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class PlayerRotationSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
