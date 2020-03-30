#include "PatrolStateSystem.h"

#include <Spatial.hpp>
#include <Navigation.hpp>

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

	entt::entity navEntity = registry.view<Navigation*>()[0];
	Navigation* pNavigation = registry.get<Navigation*>(navEntity);

	auto advanceView = registry.view<entt::tag<PatrolStateTag>, PatrolRouteComponent, Spatial*>(entt::exclude<NavPathComponent, HittedFromComponent>);
	advanceView.less([this, &registry, pNavigation] (entt::entity entity, PatrolRouteComponent& route, Spatial* pSpatial)
	{
		if (route.current < route.routePoints.size() - 1)
			route.current++;
		else
			route.current = 0;

		NavPathComponent& newPath = registry.assign<NavPathComponent>(entity);
		newPath.pathIndex = 0;
		//TODO: make nav system to target to the floor of the point
		newPath.path = pNavigation->get_simple_path(pSpatial->get_global_transform().origin, route.GetCurrentPatrolPoint());
	});

	auto lookAroundView = registry.view<HittedFromComponent>();
	lookAroundView.each([&registry, delta](entt::entity entity, HittedFromComponent& comp)
	{
		comp.lookAroundTime -= delta;
		if (comp.lookAroundTime <= 0)
			registry.remove<HittedFromComponent>(entity);
	});
}