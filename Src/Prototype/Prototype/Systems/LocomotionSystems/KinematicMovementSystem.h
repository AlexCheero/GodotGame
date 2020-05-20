#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class KinematicMovementSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
