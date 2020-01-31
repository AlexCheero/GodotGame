#include "LookAroundSystem.h"

#include <Navigation.hpp>
#include <OS.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "core/math/math_funcs.h"

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Utils/Utils.h"

bool godot::LookAroundSystem::CanSeeTarget(PlayersView& targetsView, entt::entity targetEntity
	, PatrolmanComponent patrolman, Spatial* pPatrolSpatial, float navAgentRadius)
{
	BoundsComponent targetBounds = targetsView.get<BoundsComponent>(targetEntity);
	Spatial* pTargetSpatial = targetsView.get<Spatial*>(targetEntity);

	Vector3 patrolFwd = pPatrolSpatial->get_global_transform().get_basis().z.normalized();
	Vector3 toTargetDir = pTargetSpatial->get_global_transform().origin - pPatrolSpatial->get_global_transform().origin;
	float distanceToTarget = toTargetDir.length();
	toTargetDir.normalize();
	float angle = Math::rad2deg(Math::acos(patrolFwd.dot(toTargetDir)));

	if (angle <= patrolman.viewAngleBig)
	{
		float viewDistance = angle <= patrolman.viewAngleSmall ? patrolman.longViewDistance : patrolman.shortViewDistance;
		viewDistance += navAgentRadius + targetBounds.length / 2;
		if (distanceToTarget <= viewDistance)
		{
			Object* pObj = utils::CastFromSpatial(pPatrolSpatial, patrolFwd, distanceToTarget);
			return pTargetSpatial == Object::cast_to<Spatial>(pObj);
		}
	}

	return false;
}

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
		entt::entity targetEntity = entt::null;
		for (auto entity : players)
		{
			if (CanSeeTarget(players, entity, patrolman, pSpatial, bounds.length / 2))
				targetEntity = entity;
		}

		if (registry.valid(targetEntity))
		{
			Godot::print("target found!");
			registry.remove<entt::tag<PatrollingTag> >(entity);
			PursuingComponent& pursuingComp = registry.assign<PursuingComponent>(entity, targetEntity);
			pursuingComp.targetLostMsec = -1;
			return;
		}
	});

	auto pursuingView = registry.view<PursuingComponent, PatrolmanComponent, BoundsComponent, Spatial*>(entt::exclude<entt::tag<PatrollingTag> >);
	pursuingView.each(
	[this, &registry, &players](entt::entity entity, PursuingComponent& pursuingComp, PatrolmanComponent patrolman, BoundsComponent bounds, Spatial* pSpatial)
	{
		if (!registry.valid(pursuingComp.target))
			return;
		
		if (!CanSeeTarget(players, pursuingComp.target, patrolman, pSpatial, bounds.length / 2))
		{
			int64_t ticksMsec = godot::OS::get_singleton()->get_ticks_msec();
			if (pursuingComp.targetLostMsec < 0)
				pursuingComp.targetLostMsec = ticksMsec;
			else if (pursuingComp.targetLostMsec + utils::SecondsToMillis(patrolman.looseTargetTime) <= ticksMsec)
				registry.remove<PursuingComponent>(entity);
		}
	});
}