#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class PlayerVelocitySystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}