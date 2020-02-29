#pragma once

#include "../BaseSystem.h"
#include "AttackCooldown.h"

#include "../SystemChain.h"
#include "HTHLockTargetSystem.h"
#include "../AnimSystems/HTHAnimSystem.h"

namespace godot
{
	template<>
	class SystemChain<HTHLockTargetSystem, HTHAnimSystem> : public BaseSystem
	{
	protected:
		HTHLockTargetSystem lockSystem;
		HTHAnimSystem animSystem;
	};

	class MeleeAttackSystem : public SystemChain<HTHLockTargetSystem, HTHAnimSystem>
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
