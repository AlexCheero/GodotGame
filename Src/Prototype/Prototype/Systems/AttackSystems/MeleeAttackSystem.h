#pragma once

#include "../BaseSystem.h"
#include "AttackCooldown.h"

#include "../SystemChain.h"

#include "HTHLockTargetSystem.h"
#include "PileInSystem.h"

#include "../AnimSystems/HTHAnimSystem.h"

namespace godot
{
	//TODO: SystemChain and its specializations is absolutely useless
	template<>
	class SystemChain<HTHLockTargetSystem, HTHAnimSystem> : public BaseSystem
	{
	protected:
		HTHLockTargetSystem lockSystem;
		HTHAnimSystem animSystem;
		PileInSystem pileInSystem;
	};

	class MeleeAttackSystem : public SystemChain<HTHLockTargetSystem, HTHAnimSystem>
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
