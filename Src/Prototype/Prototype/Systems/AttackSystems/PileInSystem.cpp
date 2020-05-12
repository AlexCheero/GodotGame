#include "PileInSystem.h"

#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"

void godot::PileInSystem::operator()(float delta, entt::registry& registry)
{
	auto pileInView = registry.view<PileInTag, TargetLockComponent, VelocityComponent, SpeedComponent,
									MeleeAttackComponent, Spatial*>();
	pileInView.less([&registry](entt::entity entity, TargetLockComponent lockComp,
												   VelocityComponent& velComp, SpeedComponent speedComp, MeleeAttackComponent melee, Spatial* pSpatial)
	{
		ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
		Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
		Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		
		//TODO: probably remove PileInTag in this case
		if (distanceToTarget <= melee.GetCurrentHit().minDistance)
			return;

		Vector3 toTargetVelocity = toTargetDirection;
		toTargetVelocity.y = 0;
		toTargetVelocity.normalize();
		toTargetVelocity *= speedComp.speed + speedComp.dashSpeed;
		toTargetVelocity.y = velComp.velocity.y;

		velComp.velocity = toTargetVelocity;
	});

	auto cancelPileInView = registry.view<InputComponent>();
	cancelPileInView.each([&registry, delta](entt::entity entity, InputComponent inputComp)
	{
		if (inputComp.moveDir.length_squared() > 0)
			registry.remove_if_exists<PileInTag>(entity);
	});

	auto pileInRemoveView = registry.view<PileInTag>(entt::exclude<AttackAnimPlayingComponent>);
	registry.remove<PileInTag>(pileInRemoveView.begin(), pileInRemoveView.end());
}