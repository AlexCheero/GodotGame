#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class LocomotionAnimSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
