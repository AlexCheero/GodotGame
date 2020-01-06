#include "PatrolSystem.h"

#include <Navigation.hpp>

#include "../../Components/AIComponents/NavigationComponents.h"

//TODO: smooth patrolling from point to point with bezier
void godot::PatrolSystem::operator()(float delta, entt::registry& registry)
{
	entt::entity navEntity = registry.view<Navigation*>()[0];
	Navigation* navigation = registry.get<Navigation*>(navEntity);

	auto view = registry.view<PatrolRoute, Spatial*>(entt::exclude<NavPathComponent>);
	view.each([&registry, navigation](entt::entity entity, PatrolRoute& route, Spatial* pSpatial)
	{
		if (route.current < route.routePoints.size() - 1)
			route.current++;
		else
			route.current = 0;

		NavPathComponent& newPath = registry.assign<NavPathComponent>(entity);
		newPath.pathIndex = 0;
		newPath.path = navigation->get_simple_path(pSpatial->get_global_transform().origin, route.GetCurrentPatrolPoint());
	});
}