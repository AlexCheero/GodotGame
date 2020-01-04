#pragma once

#include "../BaseSystem.h"
#include "AttackCooldown.h"

namespace godot
{
	class ThrowAttackSystem : public BaseSystem, private AttackCooldown
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}