#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class ThrowGrenadeSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
