#include "DecisionMakingFSMSystem.h"

#include <OS.hpp>

#include "core/math/math_funcs.h"

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Components/AttackComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"
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

float godot::DecisionMakingFSMSystem::GetDistanceToTarget(entt::registry& registry, entt::entity target, Spatial* pBotSpatial)
{
	ASSERT(registry.has<Spatial*>(target), "pursuing target has no spatial");
	Spatial* pTargetSpatial = registry.get<Spatial*>(target);
	//TODO: make nav system to target to the floor of the point or don't take target's y into account

	Vector3 targetPosition = pTargetSpatial->get_global_transform().origin;
	Vector3 pursuerPosition = pBotSpatial->get_global_transform().origin;
	return (targetPosition - pursuerPosition).length();
}

//TODO0: use bot tag for views
void godot::DecisionMakingFSMSystem::operator()(float delta, entt::registry& registry)
{
	auto players = registry.view<entt::tag<PlayerTag>, Spatial*>();

	//DecisionMakingFSMSystem should be the only system to manage tags and set input
	//TODO0: add noticing player on taking damage
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

		//to pursuit transition
		if (registry.valid(targetEntity))
		{
			registry.remove<entt::tag<PatrolStateTag> >(entity);
			PursuingStateComponent& pursuingComp = registry.assign<PursuingStateComponent>(entity, targetEntity);
			pursuingComp.targetLostMsec = -1;
		}
	});

	auto pursueView = registry.view<PursuingStateComponent, PatrolmanComponent, MeleeAttackComponent, HealthComponent, Spatial*>();
	pursueView.each([this, &registry, &players](entt::entity entity, PursuingStateComponent& pursuingComp
		, PatrolmanComponent patrolComp, MeleeAttackComponent meleeComp, HealthComponent healthComp, Spatial* pSpatial)
	{
		bool validTarget = registry.valid(pursuingComp.target);
		if (validTarget && CanSeeTarget(players, pursuingComp.target, patrolComp, pSpatial))
			pursuingComp.targetLostMsec = -1;
		else
		{
			int64_t ticksMsec = godot::OS::get_singleton()->get_ticks_msec();
			if (pursuingComp.targetLostMsec < 0)
				pursuingComp.targetLostMsec = ticksMsec;
			else if (pursuingComp.targetLostMsec + utils::SecondsToMillis(patrolComp.looseTargetTime) <= ticksMsec)
				validTarget = false;
		}

		//TODO0: move somwhere like DecisionMakingView
		//to flee transition
		const float criticalProportion = 0.5f;
		if (healthComp.ProportionOfMax() <= criticalProportion)
		{
			registry.remove_if_exists<NavPathComponent>(entity);
			registry.remove<PursuingStateComponent>(entity);
			registry.assign<entt::tag<FleeStateTag> >(entity);
		}
		else if (validTarget)
		{
			//to attack transition
			if (meleeComp.distance >= GetDistanceToTarget(registry, pursuingComp.target, pSpatial))
			{
				registry.remove<PursuingStateComponent>(entity);
				registry.assign<entt::tag<AttackStateTag> >(entity);
			}
		}
		//to patrol transition
		else
		{
			registry.remove<PursuingStateComponent>(entity);
			registry.assign<entt::tag<PatrolStateTag> >(entity);
		}
	});

	auto attackView = registry.view<entt::tag<AttackStateTag>, InputComponent, MeleeAttackComponent, HealthComponent, Spatial*>();
	attackView.less([this, &registry](entt::entity entity, InputComponent& inputComp, MeleeAttackComponent meleeComp
		, HealthComponent healthComp, Spatial* pSpatial)
	{
		int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
		bool attackInput = meleeComp.prevHitTimeMillis + utils::SecondsToMillis(meleeComp.attackTime) <= currTimeMillis;
		inputComp.Set(EInput::Attack, attackInput);

		//TODO0: move somwhere like DecisionMakingView here and from pursueView
		//to flee transition
		const float criticalProportion = 0.5f;
		if (healthComp.ProportionOfMax() <= criticalProportion)
		{
			registry.remove_if_exists<NavPathComponent>(entity);
			registry.remove<entt::tag<AttackStateTag> >(entity);
			registry.assign<entt::tag<FleeStateTag> >(entity);
		}
		//to pursuit transition
		if (registry.valid(meleeComp.lockedTarget) && meleeComp.distance < GetDistanceToTarget(registry, meleeComp.lockedTarget, pSpatial))
		{
			registry.remove<entt::tag<AttackStateTag> >(entity);
			PursuingStateComponent& pursuingComp = registry.assign<PursuingStateComponent>(entity, meleeComp.lockedTarget);
			pursuingComp.targetLostMsec = -1;
		}
		//to patrol transition
		else if (!registry.valid(meleeComp.lockedTarget) || registry.has<entt::tag<DeadTag> >(meleeComp.lockedTarget))
		{
			registry.remove<entt::tag<AttackStateTag> >(entity);
			registry.assign<entt::tag<PatrolStateTag> >(entity);
		}
	});

	auto fleeView = registry.view<entt::tag<FleeStateTag> >();
	fleeView.less([]()
	{
		//TODO: implement proper fleeing system
	});
}