#include "PileInSystem.h"

#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"

#include <KinematicBody.hpp>

void godot::PileInSystem::operator()(float delta, entt::registry& registry)
{
	auto checkForPileInView = registry.view<InputComponent, TargetLockComponent, MeleeAttackComponent, Spatial*>(entt::exclude<PileInTag, InAirTag>);
	checkForPileInView.each([&registry](entt::entity entity, InputComponent inputComp, TargetLockComponent lockComp, MeleeAttackComponent melee, Spatial* pSpatial)
	{
		if (!inputComp.Test(EInput::Attack))
			return;

		ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
		Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
		Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		if (distanceToTarget > melee.GetCurrentHit().maxDistance)
			registry.assign<PileInTag>(entity);
	});

	auto pileInView = registry.view<PileInTag, TargetLockComponent, VelocityComponent, SpeedComponent,
									MeleeAttackComponent, Spatial*>();
	pileInView.less([&registry](entt::entity entity, TargetLockComponent lockComp,
												   VelocityComponent& velComp, SpeedComponent speedComp, MeleeAttackComponent melee, Spatial* pSpatial)
	{
		ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
		Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
		Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		
		if (distanceToTarget <= melee.GetCurrentHit().minDistance)
			return;

		Vector3 toTargetVelocity = toTargetDirection;
		toTargetVelocity.y = 0;
		toTargetVelocity.normalize();
		toTargetVelocity *= speedComp.speed + speedComp.dashSpeed;
		toTargetVelocity.y = velComp.velocity.y;

		velComp.velocity = toTargetVelocity;
	});
}