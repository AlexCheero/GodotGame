#include "LookAroundSystem.h"

#include <Navigation.hpp>
#include <OS.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "core/math/math_funcs.h"

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Utils/Utils.h"

//TODO: check line of sight with raycst
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
			{
				PhysicsDirectSpaceState* spaceState = pPatrolSpatial->get_world()->get_direct_space_state();
				Transform attackerTransform = pPatrolSpatial->get_transform();
				Vector3 from = attackerTransform.origin;
				Vector3 to = from + attackerTransform.basis.z * distanceToTarget;
				Dictionary rayHit = spaceState->intersect_ray(from, to, Array(), utils::GetLayerByName("Player"));

				if (rayHit.empty())
					return entt::null;

				Object* pObj = Node::___get_from_variant(rayHit["collider"]);

				if (pTargetSpatial == Object::cast_to<Spatial>(pObj))
					return entity;

				return entt::null;
			}
		}
	}

	return entt::null;
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
		entt::entity targetEntity = CheckForTargets(players, patrolman, pSpatial, bounds.length / 2);
		if (registry.valid(targetEntity))
		{
			registry.remove<entt::tag<PatrollingTag> >(entity);
			PursuingComponent& pursuingComp = registry.assign<PursuingComponent>(entity, targetEntity);
			pursuingComp.targetLooseMsec = -1;
			return;
		}
	});

	//TODO: test target lost
	auto pursuingView = registry.view<entt::tag<PatrollingTag>, PursuingComponent, PatrolmanComponent, BoundsComponent, Spatial*>();
	pursuingView.less(
	[this, &registry, &players](entt::entity entity, PursuingComponent& pursuingComp, PatrolmanComponent patrolman, BoundsComponent bounds, Spatial* pSpatial)
	{
		entt::entity targetEntity = CheckForTargets(players, patrolman, pSpatial, bounds.length / 2);
		if (!registry.valid(targetEntity) && pursuingComp.targetLooseMsec >= 0)
		{
			int64_t ticksMsec = godot::OS::get_singleton()->get_ticks_msec();
			if (pursuingComp.targetLooseMsec < 0)
				pursuingComp.targetLooseMsec = ticksMsec;
			else if (pursuingComp.targetLooseMsec + utils::SecondsToMillis(pursuingComp.lostInSeconds) <= ticksMsec)
			{
				Godot::print("target lost");
				registry.remove<PursuingComponent>(entity);
			}
		}
	});
}