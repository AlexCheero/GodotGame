#include "DecisionMakingFSMSystem.h"

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"
#include "../../Components/AIComponents/PatrolComponents.h"
#include "../../Components/AttackComponents.h"

#include "../../Nodes/EntityHolderNode.h"

void godot::DecisionMakingFSMSystem::OnTransitionToPatrol(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<PursuingStateComponent>(entity);
	if (registry.has<entt::tag<MeleeAttackStateTag> >(entity))
	{
		registry.remove<entt::tag<MeleeAttackStateTag> >(entity);
		registry.remove<TargetLockComponent>(entity);
	}
}

void godot::DecisionMakingFSMSystem::OnTransitionToPursuing(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<entt::tag<PatrolStateTag> >(entity);
	if (registry.has<entt::tag<MeleeAttackStateTag> >(entity))
	{
		registry.remove<entt::tag<MeleeAttackStateTag> >(entity);
		registry.remove<TargetLockComponent>(entity);
	}
	registry.remove_if_exists<NavPathComponent>(entity);
}

void godot::DecisionMakingFSMSystem::OnTransitionToFlee(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<PursuingStateComponent>(entity);
	registry.remove_if_exists<entt::tag<MeleeAttackStateTag> >(entity);
	registry.remove_if_exists<NavPathComponent>(entity);
}

void godot::DecisionMakingFSMSystem::OnTransitionToHTH(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<PursuingStateComponent>(entity);
}

void godot::DecisionMakingFSMSystem::OnHitNoticing(entt::registry& registry)
{
	hittedOnPatrolObserver.each([&registry](const auto entity)
	{
		Vector3 hitPosition = registry.get<HittedFromComponent>(entity).position;
		registry.remove<HittedFromComponent>(entity);
		registry.remove<NavPathComponent>(entity);
		
		ASSERT(registry.has<Spatial*>(entity), "entity has no spatial");
		Spatial* pSpatial = registry.get<Spatial*>(entity);
		Vector3 dir = (hitPosition - pSpatial->get_global_transform().get_origin()).normalized();
		dir.y = 0;
		dir.normalize();

		registry.get<RotationDirectionComponent>(entity).direction = dir;

		registry.assign_or_replace<PatrolLookAroundComponent>(entity);
	});

	hittedOutOfPatrolObserver.each([&registry](const auto entity)
	{
		registry.remove<HittedFromComponent>(entity);
	});
}

godot::DecisionMakingFSMSystem::DecisionMakingFSMSystem(entt::registry& registry)
{
	registry.on_construct<PursuingStateComponent>().connect<&DecisionMakingFSMSystem::OnTransitionToPursuing>(this);
	registry.on_construct<entt::tag<PatrolStateTag> >().connect<&DecisionMakingFSMSystem::OnTransitionToPatrol>(this);
	registry.on_construct<entt::tag<FleeStateTag> >().connect<&DecisionMakingFSMSystem::OnTransitionToFlee>(this);
	registry.on_construct<entt::tag<MeleeAttackStateTag> >().connect<&DecisionMakingFSMSystem::OnTransitionToHTH>(this);
	
	hittedOnPatrolObserver.connect(registry, entt::collector.group<entt::tag<PatrolStateTag>, HittedFromComponent>());
	hittedOutOfPatrolObserver.connect(registry, entt::collector.group<HittedFromComponent>(entt::exclude<entt::tag<PatrolStateTag> >));
}

void godot::DecisionMakingFSMSystem::operator()(float delta, entt::registry& registry)
{
	//TODO: cannot use registry on_construct callback cause somehow cast fails all the time with this approach
	OnHitNoticing(registry);

	//DecisionMakingFSMSystem (or its states) should be the only system to manage states and set input for bot
	patrolSystem(delta, registry);
	pursueSystem(delta, registry);
	hthSystem(delta, registry);
	fleeSystem(delta, registry);
}