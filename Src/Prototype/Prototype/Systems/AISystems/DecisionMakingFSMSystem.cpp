#include "DecisionMakingFSMSystem.h"

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"

void godot::DecisionMakingFSMSystem::OnHitNoticing(entt::registry& registry)
{
	//TODO0: dont use local static vars, cause they become invalid on game restart
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
	
	//DecisionMakingFSMSystem (or its states) should be the only system to manage tags and set input for bot
	patrolSystem(delta, registry);
	pursueSystem(delta, registry);
	hthSystem(delta, registry);
	fleeSystem(delta, registry);
}