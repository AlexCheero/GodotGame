#include "DecisionMakingFSMSystem.h"

#include <OS.hpp>

#include "core/math/math_funcs.h"

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Components/AttackComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"

#include "../../Utils/Utils.h"

//doesn't take target's and agent's bounds into account
bool godot::DecisionMakingFSMSystem::CanSeeTarget(Spatial* pTargetSpatial, PatrolmanComponent patrolman, Spatial* pPatrolSpatial)
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

void godot::DecisionMakingFSMSystem::OnHitNoticing(entt::registry& registry)
{
	static entt::observer hittedOnPatrolObserver{ registry, entt::collector.group<entt::tag<PatrolStateTag>, HittedByComponent>() };
	hittedOnPatrolObserver.each([&registry](const auto entity)
	{
		registry.remove<entt::tag<PatrolStateTag> >(entity);
		entt::entity attacker = registry.get<HittedByComponent>(entity).attacker;
		registry.remove<HittedByComponent>(entity);
		//TODO0: NavPathComponent removed everywhere where PursuingStateComponent is assigned, refactor via reactive callbacks
		registry.remove_if_exists<NavPathComponent>(entity);
		PursuingStateComponent& pursuingComp = registry.assign<PursuingStateComponent>(entity, attacker);
		pursuingComp.targetLostMsec = -1;
	});

	static entt::observer hittedOutOfPatrolObserver{ registry, entt::collector.group<HittedByComponent>(entt::exclude<entt::tag<PatrolStateTag> >) };
	hittedOutOfPatrolObserver.each([&registry](const auto entity)
	{
		registry.remove<HittedByComponent>(entity);
	});
}

void godot::DecisionMakingFSMSystem::operator()(float delta, entt::registry& registry)
{
	OnHitNoticing(registry);
	
	auto players = registry.view<entt::tag<PlayerTag>, Spatial*>();

	//DecisionMakingFSMSystem should be the only system to manage tags and set input
	auto patrolView = registry.view<entt::tag<BotTag>, entt::tag<PatrolStateTag>, PatrolmanComponent, Spatial*>();
	patrolView.less([this, &registry, &players](entt::entity entity, PatrolmanComponent patrolComp, Spatial* pSpatial)
	{
		entt::entity targetEntity = entt::null;
		for (auto entity : players)
		{
			if (!CanSeeTarget(players.get<Spatial*>(entity), patrolComp, pSpatial))
				continue;

			targetEntity = entity;
			break;
		}

		//to pursuit transition
		if (registry.valid(targetEntity))
		{
			registry.remove<entt::tag<PatrolStateTag> >(entity);
			registry.remove_if_exists<NavPathComponent>(entity);
			PursuingStateComponent& pursuingComp = registry.assign<PursuingStateComponent>(entity, targetEntity);
			pursuingComp.targetLostMsec = -1;
		}
	});

	auto pursueView = registry.view<entt::tag<BotTag>, PursuingStateComponent, PatrolmanComponent, MeleeAttackComponent, HealthComponent, Spatial*>();
	pursueView.less([this, &registry, &players](entt::entity entity, PursuingStateComponent& pursuingComp
		, PatrolmanComponent patrolComp, MeleeAttackComponent& meleeComp, HealthComponent healthComp, Spatial* pSpatial)
	{
		bool validTarget = registry.valid(pursuingComp.target);
		if (validTarget && CanSeeTarget(players.get<Spatial*>(pursuingComp.target), patrolComp, pSpatial))
			pursuingComp.targetLostMsec = -1;
		else
		{
			int64_t ticksMsec = godot::OS::get_singleton()->get_ticks_msec();
			if (pursuingComp.targetLostMsec < 0)
				pursuingComp.targetLostMsec = ticksMsec;
			else if (pursuingComp.targetLostMsec + utils::SecondsToMillis(patrolComp.looseTargetTime) <= ticksMsec)
				validTarget = false;
		}

		//TODO: move somwhere like DecisionMakingView
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
				registry.assign<entt::tag<MeleeAttackStateTag> >(entity);
				registry.assign<TargetLockComponent>(entity).target = pursuingComp.target;
			}
		}
		//to patrol transition
		else
		{
			registry.remove<PursuingStateComponent>(entity);
			registry.assign<entt::tag<PatrolStateTag> >(entity);
		}
	});

	auto meleeAttackView = registry.view<entt::tag<BotTag>, entt::tag<MeleeAttackStateTag>, InputComponent
		, MeleeAttackComponent, HealthComponent, Spatial*
		, TargetLockComponent>();
	meleeAttackView.less([this, &registry](entt::entity entity, InputComponent& inputComp, MeleeAttackComponent& meleeComp
		, HealthComponent healthComp, Spatial* pSpatial, TargetLockComponent lockComp)
	{
		int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
		bool attackInput = meleeComp.prevHitTimeMillis + utils::SecondsToMillis(meleeComp.attackTime) <= currTimeMillis;
		inputComp.Set(EInput::Attack, attackInput);

		//TODO: move somwhere like DecisionMakingView here and from pursueView
		//to flee transition
		const float criticalProportion = 0.5f;
		if (healthComp.ProportionOfMax() <= criticalProportion)
		{
			registry.remove_if_exists<NavPathComponent>(entity);
			registry.remove<entt::tag<MeleeAttackStateTag> >(entity);
			registry.assign<entt::tag<FleeStateTag> >(entity);
		}
		//to pursuit transition
		if (registry.valid(lockComp.target) && meleeComp.distance < GetDistanceToTarget(registry, lockComp.target, pSpatial))
		{
			registry.remove_if_exists<NavPathComponent>(entity);
			PursuingStateComponent& pursuingComp = registry.assign<PursuingStateComponent>(entity, lockComp.target);
			registry.remove<entt::tag<MeleeAttackStateTag> >(entity);
			registry.remove<TargetLockComponent>(entity);
			pursuingComp.targetLostMsec = -1;
		}
		//to patrol transition
		else if (!registry.valid(lockComp.target) || registry.has<entt::tag<DeadTag> >(lockComp.target))
		{
			registry.remove<entt::tag<MeleeAttackStateTag> >(entity);
			registry.remove<TargetLockComponent>(entity);
			registry.assign<entt::tag<PatrolStateTag> >(entity);
		}
	});

	auto fleeView = registry.view<entt::tag<BotTag>, entt::tag<FleeStateTag> >();
	fleeView.less([]()
	{
		//TODO: implement proper fleeing system
	});
}