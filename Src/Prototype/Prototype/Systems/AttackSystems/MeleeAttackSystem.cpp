#include "MeleeAttackSystem.h"

#include <OS.hpp>

#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"

//TODO: implement proper hth with blocks and stuff
//TODO: call such systems only by input callback
//TODO: refactor all this mess
void godot::MeleeAttackSystem::operator()(float delta, entt::registry& registry)
{
	int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
	auto startAttackView = registry.view<CurrentWeaponMeleeTag, MeleeAttackComponent, InputComponent>(entt::exclude<AttackActionTag>);
	startAttackView.less([this, &registry, currTimeMillis](entt::entity entity, MeleeAttackComponent& attackComp, InputComponent input)
	{
		if (!input.Test(EInput::Attack) || attackComp.prevHitTimeMillis + utils::SecondsToMillis(attackComp.GetCurrentHit().attackTime) > currTimeMillis)
			return;

		int64_t millisSinceLastHit = currTimeMillis - attackComp.prevHitTimeMillis;
		attackComp.prevHitTimeMillis = currTimeMillis;

		if (millisSinceLastHit <= attackComp.maxComboIntervalMillis)
			registry.assign<IncrementComboTag>(entity);

		//TODO: locks on target on every hit, this may cause bugs with many enemies
		registry.assign<AttackActionTag>(entity);
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
			registry.remove_if_exists<PileInTag>(entity);
	});

	auto pileInRemoveOnAnimationEndView = registry.view<PileInTag>(entt::exclude<AttackAnimPlayingComponent>);
	registry.remove<PileInTag>(pileInRemoveOnAnimationEndView.begin(), pileInRemoveOnAnimationEndView.end());

	auto incrementComboView = registry.view<IncrementComboTag, MeleeAttackComponent>();
	incrementComboView.less([](MeleeAttackComponent& attackComp)
	{
		attackComp.hitIdx++;
		if (attackComp.hitIdx > attackComp.hits.size() - 1)
			attackComp.hitIdx = 0;
	});
	registry.remove<IncrementComboTag>(incrementComboView.begin(), incrementComboView.end());

	auto inputClearView = registry.view<AttackActionTag>();
	registry.remove<AttackActionTag>(inputClearView.begin(), inputClearView.end());
}