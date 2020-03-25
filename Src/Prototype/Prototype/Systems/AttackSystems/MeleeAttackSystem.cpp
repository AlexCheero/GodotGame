#include "MeleeAttackSystem.h"

#include "../../Components/AttackComponents.h"

//TODO: implement proper hth with blocks and stuff
//TODO: call such systems only by input callback
//TODO: refactor all this mess
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
			registry.assign<entt::tag<IncrementComboTag> >(entity);

		registry.assign<entt::tag<AttackActionTag> >(entity);
	});

	lockSystem(delta, registry);
	pileInSystem(delta, registry);
	animSystem(delta, registry);

	auto animPlayingView = registry.view<AttackAnimPlayingComponent>();
	animPlayingView.each([&registry, delta](entt::entity entity, AttackAnimPlayingComponent& attackPlayingComp)
	{
		attackPlayingComp.playBackTimeLeft -= delta;

		if (attackPlayingComp.playBackTimeLeft <= 0)
			registry.remove<AttackAnimPlayingComponent>(entity);
	});

	auto pileInRemoveOnMoveInputView = registry.view<InputComponent>();
	pileInRemoveOnMoveInputView.each([&registry, delta](entt::entity entity, InputComponent inputComp)
	{
		if (inputComp.moveDir.length_squared() > 0)
			registry.remove_if_exists<entt::tag<PileInTag> >(entity);
	});

	auto pileInRemoveOnAnimationEndView = registry.view<entt::tag<PileInTag> >(entt::exclude<AttackAnimPlayingComponent>);
	registry.remove<entt::tag<PileInTag> >(pileInRemoveOnAnimationEndView.begin(), pileInRemoveOnAnimationEndView.end());

	auto incrementComboView = registry.view<entt::tag<IncrementComboTag>, MeleeAttackComponent>();
	incrementComboView.less([](MeleeAttackComponent& attackComp)
	{
		attackComp.comboSequenceNum++;
		if (attackComp.comboSequenceNum > attackComp.comboLength - 1)
			attackComp.comboSequenceNum = 0;
	});
	registry.remove<entt::tag<IncrementComboTag> >(incrementComboView.begin(), incrementComboView.end());

	auto inputClearView = registry.view<entt::tag<AttackActionTag> >();
	registry.remove<entt::tag<AttackActionTag> >(inputClearView.begin(), inputClearView.end());
}