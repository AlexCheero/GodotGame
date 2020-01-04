#pragma once

#include "../BaseSystem.h"
#include "AttackCooldown.h"

namespace godot
{
	class CastAttackSystem : public BaseSystem, private AttackCooldown
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}