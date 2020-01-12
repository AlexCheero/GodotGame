#include "PatrolSystem.h"

#include <Navigation.hpp>

#include "core/math/math_funcs.h"

#include "../../Components/AIComponents/NavigationComponents.h"

entt::entity godot::PatrolSystem::CheckForTargets(PlayersView& targetsView, PatrolmanComponent patrolman, Spatial* pPatrolSpatial, float navAgentRadius)
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
				return entity;
		}
	}

	return entt::null;
}

//TODO: smooth patrolling from point to point with bezier and make smooth rotation
void godot::PatrolSystem::operator()(float delta, entt::registry& registry)
{
	entt::entity navEntity = registry.view<Navigation*>()[0];
	Navigation* pNavigation = registry.get<Navigation*>(navEntity);

	auto players = registry.view<entt::tag<PlayerTag>, BoundsComponent, Spatial*>();

	auto notPursuingView = registry.view<entt::tag<PatrollingTag>, PatrolRouteComponent, PatrolmanComponent, BoundsComponent, Spatial*>(entt::exclude<PursuingComponent>);
	notPursuingView.less(
	[this, &registry, pNavigation, &players]
	(entt::entity entity, PatrolRouteComponent& route, PatrolmanComponent patrolman, BoundsComponent bounds, Spatial* pSpatial)
	{
		entt::entity targetEntity = CheckForTargets(players, patrolman, pSpatial, bounds.length / 2);
		if (registry.valid(targetEntity))
		{
			registry.remove<entt::tag<PatrollingTag> >(entity);
			registry.assign<PursuingComponent>(entity, targetEntity);
			return;
		}
	});

	auto withoutPathview = registry.view<entt::tag<PatrollingTag>, PatrolRouteComponent, Spatial*>(entt::exclude<NavPathComponent>);
	withoutPathview.less(
	[this, &registry, pNavigation, &players]
	(entt::entity entity, PatrolRouteComponent& route, Spatial* pSpatial)
	{
		int prevPoint = route.current;
		if (route.current < route.routePoints.size() - 1)
			route.current++;
		else
			route.current = 0;

		NavPathComponent& newPath = registry.assign<NavPathComponent>(entity);
		newPath.pathIndex = 0;
		//TODO: make nav system to target to the floor of the point
		newPath.path = pNavigation->get_simple_path(pSpatial->get_global_transform().origin, route.GetCurrentPatrolPoint());
	});

	//TODO: no proper remove function, update entt
	//auto view = registry.view<A, B>();
	//registry.remove<B>(view.begin(), view.end());
	auto pathFinishedView = registry.view<entt::tag<PatrollingTag>, entt::tag<PathFinishedTag> >();
	pathFinishedView.less(
	[this, &registry, pNavigation, &players]
	(entt::entity entity)
	{
		registry.remove<entt::tag<PathFinishedTag> >(entity);
	});
}