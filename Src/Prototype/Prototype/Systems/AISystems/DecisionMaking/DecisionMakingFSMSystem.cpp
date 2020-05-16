#include "DecisionMakingFSMSystem.h"

#include "../../../Components/AIComponents/FSMStateComponents.h"
#include "../../../Components/AIComponents/NavigationComponents.h"
#include "../../../Components/AIComponents/PatrolComponents.h"
#include "../../../Components/AttackComponents.h"

void godot::DecisionMakingFSMSystem::OnTransitionToPatrol(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<PursuingStateComponent>(entity);
	if (registry.has<MeleeAttackStateTag>(entity))
	{
		registry.remove<MeleeAttackStateTag>(entity);
		registry.remove<TargetLockComponent>(entity);
	}
}

void godot::DecisionMakingFSMSystem::OnTransitionToPursuing(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<PatrolStateTag>(entity);
	if (registry.has<MeleeAttackStateTag>(entity))
	{
		registry.remove<MeleeAttackStateTag>(entity);
		registry.remove<TargetLockComponent>(entity);
	}
	registry.remove_if_exists<NavPathComponent>(entity);
}

void godot::DecisionMakingFSMSystem::OnTransitionToFlee(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<PursuingStateComponent>(entity);
	registry.remove_if_exists<MeleeAttackStateTag>(entity);
	registry.remove_if_exists<NavPathComponent>(entity);
}

void godot::DecisionMakingFSMSystem::OnTransitionToHTH(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<PursuingStateComponent>(entity);
}

void godot::DecisionMakingFSMSystem::OnHitNoticing(entt::registry& registry, entt::entity entity)
{
	if (registry.has<PatrolStateTag>(entity))
	{
		Vector3 hitPosition = registry.get<HittedFromComponent>(entity).position;
		registry.remove<NavPathComponent>(entity);
		
		ASSERT(registry.has<Spatial*>(entity), "entity has no spatial");
		Spatial* pSpatial = registry.get<Spatial*>(entity);
		Vector3 dir = (hitPosition - pSpatial->get_global_transform().get_origin()).normalized();
		dir.y = 0;
		dir.normalize();

		registry.get<RotationDirectionComponent>(entity).direction = dir;
	}
}

//TODO: probably needs clearing of whole bot input on state change
void godot::DecisionMakingFSMSystem::ResetVelocityOnStateChanged(entt::registry& registry, entt::entity entity)
{
	//not assert beacuse VelocityComponent is destroyed before any state tag destroy, on bot death
	if (registry.has<VelocityComponent>(entity))
		return;

	VelocityComponent& velComp = registry.get<VelocityComponent>(entity);
	velComp.velocity.x = velComp.velocity.z = 0;
}

godot::DecisionMakingFSMSystem::DecisionMakingFSMSystem(entt::registry& registry)
{
	registry.on_construct<PursuingStateComponent>().connect<&DecisionMakingFSMSystem::OnTransitionToPursuing>(this);
	registry.on_construct<PatrolStateTag>().connect<&DecisionMakingFSMSystem::OnTransitionToPatrol>(this);
	registry.on_construct<FleeStateTag>().connect<&DecisionMakingFSMSystem::OnTransitionToFlee>(this);
	//TODO0: bot doesn't attack!!!
	registry.on_construct<MeleeAttackStateTag>().connect<&DecisionMakingFSMSystem::OnTransitionToHTH>(this);
	registry.on_construct<HittedFromComponent>().connect<&DecisionMakingFSMSystem::OnHitNoticing>(this);

	registry.on_destroy<PatrolStateTag>().connect<&entt::registry::remove_if_exists<HittedFromComponent> >();

	//TODO0: check why we even need to clear velocity. maybe should zero velocity after KinematicMovementSystem run
	//clear velocity on state change
	registry.on_construct<PursuingStateComponent>().connect<&DecisionMakingFSMSystem::ResetVelocityOnStateChanged>(this);
	registry.on_construct<PatrolStateTag>().connect<&DecisionMakingFSMSystem::ResetVelocityOnStateChanged>(this);
	registry.on_construct<FleeStateTag>().connect<&DecisionMakingFSMSystem::ResetVelocityOnStateChanged>(this);
	registry.on_construct<MeleeAttackStateTag>().connect<&DecisionMakingFSMSystem::ResetVelocityOnStateChanged>(this);
	registry.on_construct<HittedFromComponent>().connect<&DecisionMakingFSMSystem::ResetVelocityOnStateChanged>(this);
}

void godot::DecisionMakingFSMSystem::operator()(float delta, entt::registry& registry)
{
	//DecisionMakingFSMSystem (or its states) should be the only system to manage states and set input for bot
	patrolSystem(delta, registry);
	pursueSystem(delta, registry);
	hthSystem(delta, registry);
	fleeSystem(delta, registry);
}