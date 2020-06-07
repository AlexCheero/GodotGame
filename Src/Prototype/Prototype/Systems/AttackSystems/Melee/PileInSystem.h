#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class PileInSystem
	{
	public:
		static void Init(entt::registry& registry);
		static void Tick(float delta, entt::registry& registry);
	};
}
