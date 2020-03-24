#include "DecisionMakingFSMSystem.h"

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"
#include "../../Components/AttackComponents.h"

void godot::DecisionMakingFSMSystem::OnTransitionToPatrol(entt::registry& registry, entt::entity entity)
{
	if (registry.has<PursuingStateComponent>(entity))
	{
		registry.remove<PursuingStateComponent>(entity);
	}
	if (registry.has<entt::tag<MeleeAttackStateTag> >(entity))
	{
		registry.remove<entt::tag<MeleeAttackStateTag> >(entity);
		registry.remove<TargetLockComponent>(entity);
	}
}

void godot::DecisionMakingFSMSystem::OnTransitionToPursuing(entt::registry& registry, entt::entity entity)
{
	if (registry.has<entt::tag<PatrolStateTag> >(entity))
	{
		registry.remove<entt::tag<PatrolStateTag> >(entity);
		registry.remove_if_exists<NavPathComponent>(entity);
	}
	if (registry.has<entt::tag<MeleeAttackStateTag> >(entity))
	{
		registry.remove<entt::tag<MeleeAttackStateTag> >(entity);
		registry.remove_if_exists<NavPathComponent>(entity);
		registry.remove<TargetLockComponent>(entity);
	}
}

void godot::DecisionMakingFSMSystem::OnTransitionToFlee(entt::registry& registry, entt::entity entity)
{
	if (registry.has<PursuingStateComponent>(entity))
	{
		registry.remove<PursuingStateComponent>(entity);
		registry.remove_if_exists<NavPathComponent>(entity);
	}
	if (registry.has<entt::tag<MeleeAttackStateTag> >(entity))
	{
		registry.remove<entt::tag<MeleeAttackStateTag> >(entity);
		registry.remove_if_exists<NavPathComponent>(entity);
	}
}

void godot::DecisionMakingFSMSystem::OnTransitionToHTH(entt::registry& registry, entt::entity entity)
{
	if (registry.has<PursuingStateComponent>(entity))
	{
		registry.remove<PursuingStateComponent>(entity);
	}
}

void godot::DecisionMakingFSMSystem::OnHitNoticing(entt::registry& registry)
{
	hittedOnPatrolObserver.each([&registry](const auto entity)
	{
		entt::entity attacker = registry.get<HittedByComponent>(entity).attacker;
		registry.remove<HittedByComponent>(entity);
		registry.assign<PursuingStateComponent>(entity, attacker);
	});

	hittedOutOfPatrolObserver.each([&registry](const auto entity)
	{
		registry.remove<HittedByComponent>(entity);
	});
}

godot::DecisionMakingFSMSystem::DecisionMakingFSMSystem(entt::registry& registry)
{
	registry.on_construct<PursuingStateComponent>().connect<&DecisionMakingFSMSystem::OnTransitionToPursuing>(this);
	registry.on_construct<entt::tag<PatrolStateTag> >().connect<&DecisionMakingFSMSystem::OnTransitionToPatrol>(this);
	registry.on_construct<entt::tag<FleeStateTag> >().connect<&DecisionMakingFSMSystem::OnTransitionToFlee>(this);
	registry.on_construct<entt::tag<MeleeAttackStateTag> >().connect<&DecisionMakingFSMSystem::OnTransitionToHTH>(this);
	registry.on_construct<HittedByComponent>().connect<&DecisionMakingFSMSystem::OnHitNoticing>(this);

	hittedOnPatrolObserver.connect(registry, entt::collector.group<entt::tag<PatrolStateTag>, HittedByComponent>());
	hittedOutOfPatrolObserver.connect(registry, entt::collector.group<HittedByComponent>(entt::exclude<entt::tag<PatrolStateTag> >));
}

void godot::DecisionMakingFSMSystem::operator()(float delta, entt::registry& registry)
{
	//cannot use registry on_construct callback cause HittedByComponent::attacker
	//is set to already deleted entity in HTHDamagingArea
	OnHitNoticing(registry);

	//DecisionMakingFSMSystem (or its states) should be the only system to manage states and set input for bot
	patrolSystem(delta, registry);
	pursueSystem(delta, registry);
	hthSystem(delta, registry);
	fleeSystem(delta, registry);
}