#pragma once

#include "../BaseSystem.h"

namespace godot
{
	class UpdateLockRotationSystem : public BaseSystem
	{
	private:
		bool ChecktargetEntity(entt::registry& registry, entt::entity lockedTarget);
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
#pragma once
