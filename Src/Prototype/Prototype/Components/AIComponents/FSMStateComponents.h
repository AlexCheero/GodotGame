#pragma once

#include <core/CoreTypes.hpp>

#include "../ComponentsMeta.h"

#include "entt/entt.hpp"

REGISTRABLE_COMPONENT(DecisionMakingComponent)
{
	float criticalHp;
	float targetPosRecalcDistance;
};
COMPONENTS_META(DecisionMakingComponent, "criticalHp", "targetPosRecalcDistance");

struct PursuingStateComponent
{
	entt::entity target = entt::null;
	godot::Vector3 previousTargetPosition;
	int64_t targetLostMsec = -1;
};

struct HittedFromComponent
{
	godot::Vector3 position;
	float lookAroundTime;
};

TAG(PatrolStateTag);
TAG(MeleeAttackStateTag);
TAG(FleeStateTag);