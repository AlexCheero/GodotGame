#pragma once

#include "../BaseSystem.h"

#include "HTHLockTargetSystem.h"
#include "PileInSystem.h"

#include "../AnimSystems/HTHAnimSystem.h"

namespace godot
{
	class MeleeAttackSystem : public BaseSystem
	{
	private:
		HTHLockTargetSystem lockSystem;
		HTHAnimSystem animSystem;
		PileInSystem pileInSystem;
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
