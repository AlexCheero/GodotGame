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

		ASSERT(registry.has<Spatial*>(entity), "entity has no spatial");
		Spatial* pSpatial = registry.get<Spatial*>(entity);
		ASSERT(registry.has<PatrolmanComponent>(entity), "entity has no PatrolmanComponent");
		PatrolmanComponent patrolmanComp = registry.get<PatrolmanComponent>(entity);
		Vector3 dir = (hitPosition - pSpatial->get_global_transform().get_origin()).normalized();
		Object* pHitted = utils::CastFromSpatial(pSpatial, dir, patrolmanComp.longViewDistance);

		if (!pHitted)
			return;

		EntityHolderNode* pEntityHolder = Object::cast_to<EntityHolderNode>(pHitted);
		if (!pEntityHolder)
			return;

		ASSERT(registry.valid(pEntityHolder->GetEntity()), "invalid entity");
		registry.assign<PursuingStateComponent>(entity, pEntityHolder->GetEntity());
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
	registry.on_construct<HittedFromComponent>().connect<&DecisionMakingFSMSystem::OnHitNoticing>(this);

	hittedOnPatrolObserver.connect(registry, entt::collector.group<entt::tag<PatrolStateTag>, HittedFromComponent>());
	hittedOutOfPatrolObserver.connect(registry, entt::collector.group<HittedFromComponent>(entt::exclude<entt::tag<PatrolStateTag> >));
}

void godot::DecisionMakingFSMSystem::operator()(float delta, entt::registry& registry)
{
	//cannot use registry on_construct callback cause HittedByComponent::attacker
	//is set to already deleted entity in HTHDamagingArea
	//TODO: try to use registry on_construct callback after implementing HittedFromComponent
	OnHitNoticing(registry);

	//DecisionMakingFSMSystem (or its states) should be the only system to manage states and set input for bot
	patrolSystem(delta, registry);
	pursueSystem(delta, registry);
	hthSystem(delta, registry);
	fleeSystem(delta, registry);
}