#include "PatrolSystem.h"

#include <Navigation.hpp>

#include "core/math/math_funcs.h"

#include "../../Components/AIComponents/NavigationComponents.h"

//TODO: probably split into two separate systems
bool godot::PatrolSystem::CheckForTargets(PlayersView& targetsView, PatrolmanComponent patrolman, Spatial* pPatrolSpatial, float navAgentRadius)
{
	for (auto entity : targetsView)
	{
		BoundsComponent bounds = targetsView.get<BoundsComponent>(entity);
		Spatial* pTargetSpatial = targetsView.get<Spatial*>(entity);
		Vector3 patrolFwd = pPatrolSpatial->get_global_transform().get_basis().z.normalized();
		Vector3 toTargetDir = pTargetSpatial->get_global_transform().origin - pPatrolSpatial->get_global_transform().origin;
		float distanceToTarget = toTargetDir.length();
		toTargetDir.normalize();
		float angle = Math::rad2deg(Math::acos(patrolFwd.dot(toTargetDir)));

		if (angle <= patrolman.viewAngleBig)
		{
			float viewDistance = angle <= patrolman.viewAngleSmall ? patrolman.longViewDistance : patrolman.shortViewDistance;
			viewDistance += navAgentRadius + bounds.length / 2;
			if (distanceToTarget <= viewDistance)
				//TODO: return noticed target
				return true;
		}
	}

	return false;
}

//TODO: smooth patrolling from point to point with bezier and make smooth rotation
void godot::PatrolSystem::operator()(float delta, entt::registry& registry)
{
	entt::entity navEntity = registry.view<Navigation*>()[0];
	Navigation* navigation = registry.get<Navigation*>(navEntity);

	auto players = registry.view<entt::tag<PlayerTag>, BoundsComponent, Spatial*>();

	auto view = registry.view<PatrolRouteComponent, PatrolmanComponent, BoundsComponent, Spatial*>(entt::exclude<NavPathComponent>);
	view.each(
	[this, &registry, navigation, &players]
	(entt::entity entity, PatrolRouteComponent& route, PatrolmanComponent patrolman, BoundsComponent bounds, Spatial* pSpatial)
	{
		static bool prevHaveTarget = false;
		bool haveTarget = CheckForTargets(players, patrolman, pSpatial, bounds.length / 2);

		if (prevHaveTarget != haveTarget)
		{
			Godot::print(String("target: ") + (haveTarget ? "true" : "false"));
			prevHaveTarget = haveTarget;
		}

		return;

		if (route.current < route.routePoints.size() - 1)
			route.current++;
		else
			route.current = 0;

		NavPathComponent& newPath = registry.assign<NavPathComponent>(entity);
		newPath.pathIndex = 0;
		//TODO: make nav system to target to the floor of the point
		newPath.path = navigation->get_simple_path(pSpatial->get_global_transform().origin, route.GetCurrentPatrolPoint());
	});
}