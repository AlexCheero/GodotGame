#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class MeleeAnimSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
