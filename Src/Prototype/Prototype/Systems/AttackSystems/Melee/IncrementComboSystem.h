#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class IncrementComboSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
