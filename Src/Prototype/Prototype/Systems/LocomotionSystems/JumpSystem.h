#pragma once

#include "entt/entt.hpp"

namespace godot
{
	namespace ReactiveJumpSystem
	{
		void Init(entt::registry& registry);
		void OnInputPressed(entt::registry& registry, entt::entity entity);
	}
}
