#include "PatrolSystem.h"

#include <Navigation.hpp>

#include "core/math/math_funcs.h"

#include "../../Components/AIComponents/NavigationComponents.h"

//TODO: probably split into two separate systems
bool godot::PatrolSystem::CheckForTargets(PlayersView& targetsView, Spatial* pPatrolSpatial)
{
	for (auto entity : targetsView)
	{
		Spatial* pTargetSpatial = targetsView.get<Spatial*>(entity);
		//TODO: make fields (instead of consts) in view class and take target and patrolman radius into account
		const float angleSmall = 45;
		const float angleBig = 90;
		Vector3 patrolFwd = pPatrolSpatial->get_global_transform().get_basis().z.normalized();
		Vector3 toTargetDir = pTargetSpatial->get_global_transform().origin - pPatrolSpatial->get_global_transform().origin;
		float distanceToTarget = toTargetDir.length();
		toTargetDir.normalize();
		float angle = Math::rad2deg(Math::acos(patrolFwd.dot(toTargetDir)));

		if (angle <= angleBig)
		{
			const float longViewDistance = 30.f;
			const float shortViewDistance = 15.f;
			float viewDistance = angle <= angleSmall ? longViewDistance : shortViewDistance;
			if (distanceToTarget <= viewDistance)
				//TODO: return noticed target
				return true;
		}
	}

	return false;
}

//TODO: smooth patrolling from point to point with bezier
void godot::PatrolSystem::operator()(float delta, entt::registry& registry)
{
	entt::entity navEntity = registry.view<Navigation*>()[0];
	Navigation* navigation = registry.get<Navigation*>(navEntity);

	auto players = registry.view<entt::tag<PlayerTag>, Spatial*>();

	auto view = registry.view<PatrolRoute, Spatial*>(entt::exclude<NavPathComponent>);
	view.each([this, &registry, navigation, &players](entt::entity entity, PatrolRoute& route, Spatial* pSpatial)
	{
		CheckForTargets(players, pSpatial);
		return;

		if (route.current < route.routePoints.size() - 1)
			route.current++;
		else
			route.current = 0;

		NavPathComponent& newPath = registry.assign<NavPathComponent>(entity);
		newPath.pathIndex = 0;
		newPath.path = navigation->get_simple_path(pSpatial->get_global_transform().origin, route.GetCurrentPatrolPoint());
	});
}