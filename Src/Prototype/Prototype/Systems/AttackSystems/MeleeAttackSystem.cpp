#include "MeleeAttackSystem.h"

#include "../../Components/AttackComponents.h"

//TODO: implement proper hth with blocks and stuff
//TODO: call such systems only by input callback
void godot::MeleeAttackSystem::operator()(float delta, entt::registry& registry)
{
	int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
	auto startAttackView = registry.view<entt::tag<CurrentWeaponMeleeTag>, MeleeAttackComponent, InputComponent>(entt::exclude<entt::tag<AttackActionTag> >);
	startAttackView.less([this, &registry, currTimeMillis](entt::entity entity, MeleeAttackComponent& attackComp, InputComponent input)
	{
		if (!input.Test(EInput::Attack) || attackComp.prevHitTimeMillis + utils::SecondsToMillis(attackComp.attackTime) > currTimeMillis)
			return;

		int64_t millisSinceLastHit = currTimeMillis - attackComp.prevHitTimeMillis;
		attackComp.prevHitTimeMillis = currTimeMillis;

		if (millisSinceLastHit <= attackComp.maxComboIntervalMillis)
		{
			attackComp.comboSequenceNum++;
			if (attackComp.comboSequenceNum > attackComp.comboLength - 1)
				attackComp.comboSequenceNum = 0;
		}

		registry.assign<entt::tag<AttackActionTag> >(entity);
	});

	lockSystem(delta, registry);

	//TODO0: it difficult to hit the moving target anyway, needs some different logick
//=====================PileInSystem=====================
	//TODO: proably move to pile in system
	//TODO: remove hardcode
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

	//TODO0: pile in only for a few steps
	auto pileInView = registry.view<entt::tag<PileInTag>, TargetLockComponent, VelocityComponent, SpeedComponent, Spatial*>();
	pileInView.less([&registry, maxAttackDistance](entt::entity entity, TargetLockComponent lockComp,
												   VelocityComponent& velComp, SpeedComponent speedComp, Spatial* pSpatial)
	{
		ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
		Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
		Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		if (distanceToTarget <= maxAttackDistance)
		{
			registry.remove<entt::tag<PileInTag> >(entity);
			return;
		}

		Vector3 toTargetVelocity = toTargetDirection;
		toTargetVelocity.y = 0;
		toTargetVelocity.normalize();
		//TODO: remove hardcode and probably use speed instead of factor
		const float pileInDashFactor = 1.5f;
		velComp.velocity += toTargetVelocity * speedComp.speed * pileInDashFactor;
	});
//======================================================

	animSystem(delta, registry);

	auto inputClearView = registry.view<entt::tag<AttackActionTag> >();
	registry.remove<entt::tag<AttackActionTag> >(inputClearView.begin(), inputClearView.end());
}