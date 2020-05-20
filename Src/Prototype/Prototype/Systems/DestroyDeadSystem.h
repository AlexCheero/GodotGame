#pragma once

#include "entt/entt.hpp"

#include "../Components/SimpleComponents.h"

namespace godot
{
	class DestroyDeadSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
