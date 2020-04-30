#include "DecisionMakingHelper.h"

#include "core/math/math_funcs.h"

#include "../../../Utils/Utils.h"

//doesn't take target's and agent's bounds into account
bool godot::CanSeeTarget(Spatial* pTargetSpatial, PatrolmanComponent patrolman, Spatial* pPatrolSpatial)
{
	Vector3 patrolFwd = pPatrolSpatial->get_global_transform().get_basis().z.normalized();
	Vector3 toTargetDir = pTargetSpatial->get_global_transform().origin - pPatrolSpatial->get_global_transform().origin;
	float distanceToTarget = toTargetDir.length();
	toTargetDir.normalize();
	float angle = Math::rad2deg(Math::acos(patrolFwd.dot(toTargetDir)));

	if (angle <= patrolman.viewAngleBig)
	{
		float viewDistance = angle <= patrolman.viewAngleSmall ? patrolman.longViewDistance : patrolman.shortViewDistance;
		if (distanceToTarget <= viewDistance)
			return pTargetSpatial == utils::CastFromSpatial(pPatrolSpatial, patrolFwd, distanceToTarget);
	}

	return false;
}

float godot::GetDistanceToTarget(entt::registry& registry, entt::entity target, Spatial* pBotSpatial)
{
	ASSERT(registry.has<Spatial*>(target), "pursuing target has no spatial");
	Spatial* pTargetSpatial = registry.get<Spatial*>(target);
	Vector3 targetPosition = pTargetSpatial->get_global_transform().origin;
	Vector3 pursuerPosition = pBotSpatial->get_global_transform().origin;
	return (targetPosition - pursuerPosition).length();
}