#include "PileInSystem.h"

#include "../../Components/AttackComponents.h"

void godot::PileInSystem::operator()(float delta, entt::registry& registry)
{
	//TODO: remove hardcode and calculate separate maxAttackDistance for each hit
	//TODO: player + bot colliders radii is hardcoded also
	const float maxAttackDistance = 2.f;//1.5f
	auto checkForPileInView = registry.view<entt::tag<AttackActionTag>, TargetLockComponent,
											VelocityComponent, SpeedComponent, Spatial*>(entt::exclude<entt::tag<PileInTag> >);
	checkForPileInView.less([&registry, maxAttackDistance](entt::entity entity, TargetLockComponent lockComp, VelocityComponent velComp,
														   SpeedComponent speedComp, Spatial* pSpatial)
	{
		ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
		Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
		Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		if (distanceToTarget > maxAttackDistance)
			registry.assign<entt::tag<PileInTag> >(entity);
	});

	//TODO0: stop or slow down fleeing enemy while in hth combat, not always pile in
	auto pileInView = registry.view<entt::tag<PileInTag>, AttackAnimPlayingComponent, TargetLockComponent, VelocityComponent, SpeedComponent, Spatial*>();
	pileInView.less([&registry, maxAttackDistance](entt::entity entity, AttackAnimPlayingComponent animPlayingComp, TargetLockComponent lockComp,
												   VelocityComponent& velComp, SpeedComponent speedComp, Spatial* pSpatial)
	{
		ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
		Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
		Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		
		//TODO: remove this hardcode also
		if (distanceToTarget <= maxAttackDistance / 2)
			return;

		Vector3 toTargetVelocity = toTargetDirection;
		toTargetVelocity.y = 0;
		toTargetVelocity.normalize();
		//TODO: remove hardcode and probably use speed instead of factor
		const float pileInDashFactor = 1.5f;
		velComp.velocity += toTargetVelocity * speedComp.speed * pileInDashFactor;
	});
}