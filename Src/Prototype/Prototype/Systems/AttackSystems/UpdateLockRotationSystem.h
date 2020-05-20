#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class UpdateLockRotationSystem
	{
	private:
		static bool ChecktargetEntity(entt::registry& registry, entt::entity lockedTarget);
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
#pragma once
