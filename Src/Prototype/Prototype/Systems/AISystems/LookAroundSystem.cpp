#include "LookAroundSystem.h"

#include <Navigation.hpp>

#include "core/math/math_funcs.h"

#include "../../Components/AIComponents/FSMStateComponents.h"

entt::entity godot::LookAroundSystem::CheckForTargets(PlayersView& targetsView, PatrolmanComponent patrolman, Spatial* pPatrolSpatial, float navAgentRadius)
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

//TODO: implement losing sight of target
void godot::LookAroundSystem::operator()(float delta, entt::registry& registry)
{
	entt::entity navEntity = registry.view<Navigation*>()[0];
	Navigation* pNavigation = registry.get<Navigation*>(navEntity);

	auto players = registry.view<entt::tag<PlayerTag>, BoundsComponent, Spatial*>();

	auto view = registry.view<entt::tag<PatrollingTag>, PatrolmanComponent, BoundsComponent, Spatial*>(entt::exclude<PursuingComponent>);
	view.less(
	[this, &registry, pNavigation, &players]
	(entt::entity entity, PatrolmanComponent patrolman, BoundsComponent bounds, Spatial* pSpatial)
	{
		entt::entity targetEntity = CheckForTargets(players, patrolman, pSpatial, bounds.length / 2);
		if (registry.valid(targetEntity))
		{
			registry.remove<entt::tag<PatrollingTag> >(entity);
			registry.assign<PursuingComponent>(entity, targetEntity);
			return;
		}
	});
}