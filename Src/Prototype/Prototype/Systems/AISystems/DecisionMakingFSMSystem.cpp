#include "DecisionMakingFSMSystem.h"

#include <OS.hpp>

#include "core/math/math_funcs.h"

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Utils/Utils.h"

//doesn't take target's and agent's bounds into account
bool godot::DecisionMakingFSMSystem::CanSeeTarget(PlayersView& targetsView, entt::entity targetEntity
	, PatrolmanComponent patrolman, Spatial* pPatrolSpatial)
{
	Spatial* pTargetSpatial = targetsView.get<Spatial*>(targetEntity);

	Vector3 patrolFwd = pPatrolSpatial->get_global_transform().get_basis().z.normalized();
	Vector3 toTargetDir = pTargetSpatial->get_global_transform().origin - pPatrolSpatial->get_global_transform().origin;
	float distanceToTarget = toTargetDir.length();
	toTargetDir.normalize();
	float angle = Math::rad2deg(Math::acos(patrolFwd.dot(toTargetDir)));

	if (angle <= patrolman.viewAngleBig)
	{
		float viewDistance = angle <= patrolman.viewAngleSmall ? patrolman.longViewDistance : patrolman.shortViewDistance;
		if (distanceToTarget <= viewDistance)
		{
			Object* pObj = utils::CastFromSpatial(pPatrolSpatial, patrolFwd, distanceToTarget);
			return pTargetSpatial == Object::cast_to<Spatial>(pObj);
		}
	}

	return false;
}

void godot::DecisionMakingFSMSystem::operator()(float delta, entt::registry& registry)
{
	auto players = registry.view<entt::tag<PlayerTag>, Spatial*>();

	//DecisionMakingFSMSystem should be the only system to manage tags
	auto patrolView = registry.view<entt::tag<PatrolStateTag>, PatrolmanComponent, Spatial*>();
	patrolView.less([this, &registry, &players](entt::entity entity, PatrolmanComponent patrolComp, Spatial* pSpatial)
	{
		entt::entity targetEntity = entt::null;
		for (auto entity : players)
		{
			if (!CanSeeTarget(players, entity, patrolComp, pSpatial))
				continue;

			targetEntity = entity;
			break;
		}

		if (registry.valid(targetEntity))
		{
			Godot::print("target found!");
			registry.remove<entt::tag<PatrolStateTag> >(entity);
			PursuingStateComponent& pursuingComp = registry.assign<PursuingStateComponent>(entity, targetEntity);
			pursuingComp.targetLostMsec = -1;
		}
	});

	auto pursueView = registry.view<PursuingStateComponent, PatrolmanComponent, Spatial*>();
	pursueView.each([this, &registry, &players](entt::entity entity, PursuingStateComponent pursuingComp, PatrolmanComponent patrolComp, Spatial* pSpatial)
	{
		if (!registry.valid(pursuingComp.target))
			return;
		
		if (!CanSeeTarget(players, pursuingComp.target, patrolComp, pSpatial))
		{
			int64_t ticksMsec = godot::OS::get_singleton()->get_ticks_msec();
			if (pursuingComp.targetLostMsec < 0)
				pursuingComp.targetLostMsec = ticksMsec;
			else if (pursuingComp.targetLostMsec + utils::SecondsToMillis(patrolComp.looseTargetTime) <= ticksMsec)
			{
				registry.remove<PursuingStateComponent>(entity);
				registry.assign<entt::tag<PatrolStateTag> >(entity);
			}
		}
	});

	auto fleeView = registry.view<entt::tag<FleeStateTag> >();
	fleeView.less([]()
	{
		//
	});

	auto attackView = registry.view<entt::tag<AttackStateTag> >();
	attackView.less([]()
	{
		//
	});
}