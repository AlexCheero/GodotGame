#include "PatrolStateSystem.h"

#include <Spatial.hpp>

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/PatrolComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"

#include "DecisionMakingHelper.h"

godot::PatrolStateSystem::PatrolStateSystem(entt::registry& registry)
{
	toPursuitTransitionObserver.connect(registry, entt::collector.group<PursuingStateComponent>().where<entt::tag<PatrolStateTag> >());
}

void godot::PatrolStateSystem::operator()(float delta, entt::registry& registry)
{
	auto players = registry.view<entt::tag<PlayerTag>, Spatial*>();
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
			registry.assign<PursuingStateComponent>(entity, targetEntity);
	});

	toPursuitTransitionObserver.each([&registry](const auto entity)
	{
		registry.remove<entt::tag<PatrolStateTag> >(entity);
		registry.remove_if_exists<NavPathComponent>(entity);
	});
}