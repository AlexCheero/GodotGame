#include "StartMeleeAttackSystem.h"

#include <OS.hpp>

#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"

void godot::StartMeleeAttackSystem::operator()(float delta, entt::registry& registry)
{
	int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
	auto startAttackView = registry.view<AttackPressedTag, CurrentWeaponMeleeTag, MeleeAttackComponent>(entt::exclude<AttackAnimPlayingComponent>);
	startAttackView.less([this, &registry, currTimeMillis](entt::entity entity, MeleeAttackComponent& attackComp)
	{
		if (attackComp.prevHitTimeMillis + utils::SecondsToMillis(attackComp.GetCurrentHit().attackTime) > currTimeMillis)
		{
			registry.remove<AttackPressedTag>(entity);
			return;
		}

		int64_t millisSinceLastHit = currTimeMillis - attackComp.prevHitTimeMillis;
		attackComp.prevHitTimeMillis = currTimeMillis;

		//TODO0: increment bug, when attack is pressed before animation stops playing
		if (millisSinceLastHit <= MeleeAttackComponent::maxComboIntervalMillis)
			registry.assign<IncrementComboTag>(entity);
	});
}