#pragma once

#include "entt/entt.hpp"

#include "../../Components/SimpleComponents.h"

namespace godot
{
	class LookAtSystem
	{
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
