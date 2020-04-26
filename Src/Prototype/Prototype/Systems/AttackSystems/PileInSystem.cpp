#include "PileInSystem.h"

#include "../../Components/AttackComponents.h"

void godot::PileInSystem::operator()(float delta, entt::registry& registry)
{
	auto checkForPileInView = registry.view<AttackActionTag, TargetLockComponent, MeleeAttackComponent, Spatial*>(entt::exclude<PileInTag>);
	checkForPileInView.less([&registry](entt::entity entity, TargetLockComponent lockComp, MeleeAttackComponent melee, Spatial* pSpatial)
	{
		ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
		Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
		Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		if (distanceToTarget > melee.maxDistance)
			registry.assign<PileInTag>(entity);
	});

	auto pileInView = registry.view<PileInTag, AttackAnimPlayingComponent, TargetLockComponent, VelocityComponent, SpeedComponent,
									MeleeAttackComponent, Spatial*>();
	pileInView.less([&registry](entt::entity entity, AttackAnimPlayingComponent animPlayingComp, TargetLockComponent lockComp,
												   VelocityComponent& velComp, SpeedComponent speedComp, MeleeAttackComponent melee, Spatial* pSpatial)
	{
		ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
		Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
		Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		
		if (distanceToTarget <= melee.minDistance)
			return;

		Vector3 toTargetVelocity = toTargetDirection;
		toTargetVelocity.y = 0;
		toTargetVelocity.normalize();
		toTargetVelocity *= speedComp.speed + melee.dashSpeed;
		//TODO: don't pile in in air, not just reset velocity's y
		//      it slows down drastically if using is_on_floor
		toTargetVelocity.y = velComp.velocity.y;

		velComp.velocity = toTargetVelocity;
	});
}