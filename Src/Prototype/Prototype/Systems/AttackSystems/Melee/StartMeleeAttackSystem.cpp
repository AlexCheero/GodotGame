#include "StartMeleeAttackSystem.h"

#include <OS.hpp>

#include "../../../Components/AttackComponents.h"
#include "../../../Components/InputComponents.h"

//TODO: convert to attack cooldown check. probably remove utils::Expired after converting
void godot::StartMeleeAttackSystem::operator()(float delta, entt::registry& registry)
{
	int64_t currTimeMillis = OS::get_singleton()->get_ticks_msec();
	auto view = registry.view<AttackPressedTag, CurrentWeaponMeleeTag, MeleeAttackComponent>();
	view.less([&registry, currTimeMillis](entt::entity entity, MeleeAttackComponent& attackComp)
	{
		if (attackComp.prevHitTimeMillis + utils::SecondsToMillis(attackComp.GetCurrentHit().attackTime) <= currTimeMillis)
		{
			int64_t millisSinceLastHit = currTimeMillis - attackComp.prevHitTimeMillis;
			attackComp.prevHitTimeMillis = currTimeMillis;
		}
		//TODO: accumulate next input for continuing combo
		else
			registry.remove<AttackPressedTag>(entity);
	});
}