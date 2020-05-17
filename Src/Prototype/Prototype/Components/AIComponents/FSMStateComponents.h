#pragma once

#include <core/CoreTypes.hpp>

#include "../ComponentsMeta.h"

#include "entt/entt.hpp"

struct DecisionMakingComponent
{
	float criticalHp;
	float targetPosRecalcDistance;
};
REGISTER_COMPONENT(DecisionMakingComponent, "criticalHp", "targetPosRecalcDistance");

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

DECLARE_TAG(PatrolStateTag);
DECLARE_TAG(MeleeAttackStateTag);
DECLARE_TAG(FleeStateTag);