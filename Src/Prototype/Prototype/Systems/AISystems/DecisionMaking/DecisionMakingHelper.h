#pragma once

#include "entt/entt.hpp"

#include <Spatial.hpp>

#include "../../../Components/AIComponents/PatrolComponents.h"

namespace godot
{
	bool CanSeeTarget(Spatial* pTargetSpatial, PatrolmanComponent patrolman, Spatial* pPatrolSpatial);
	float GetDistanceToTarget(entt::registry& registry, entt::entity target, Spatial* pBotSpatial);
}
