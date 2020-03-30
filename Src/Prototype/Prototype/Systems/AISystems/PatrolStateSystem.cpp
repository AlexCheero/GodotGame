#include "PatrolStateSystem.h"

#include <Spatial.hpp>

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/PatrolComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"

#include "DecisionMakingHelper.h"

//TODO: merge PatrolStateSystem and PatrolSystem
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
}