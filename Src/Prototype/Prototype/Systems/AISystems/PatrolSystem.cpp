#include "PatrolSystem.h"

#include <Navigation.hpp>

#include "../../Components/AIComponents/PatrolComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"
#include "../../Components/AIComponents/FSMStateComponents.h"

//TODO: smooth patrolling from point to point with bezier and make smooth rotation
void godot::PatrolSystem::operator()(float delta, entt::registry& registry)
{
	entt::entity navEntity = registry.view<Navigation*>()[0];
	Navigation* pNavigation = registry.get<Navigation*>(navEntity);

	auto withoutPathview = registry.view<entt::tag<PatrollingTag>, PatrolRouteComponent, Spatial*>(entt::exclude<NavPathComponent>);
	withoutPathview.less(
	[this, &registry, pNavigation]
	(entt::entity entity, PatrolRouteComponent& route, Spatial* pSpatial)
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

	auto pathFinishedView = registry.view<entt::tag<PatrollingTag>, entt::tag<PathFinishedTag> >();
	registry.remove<entt::tag<PathFinishedTag> >(pathFinishedView.begin(), pathFinishedView.end());
}