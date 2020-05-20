#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class PursuingSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
