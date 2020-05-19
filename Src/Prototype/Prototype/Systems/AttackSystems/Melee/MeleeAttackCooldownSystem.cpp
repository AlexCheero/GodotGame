#include "MeleeAttackCooldownSystem.h"

#include <OS.hpp>

#include "../../../Components/AttackComponents.h"
#include "../../../Components/InputComponents.h"

void godot::MeleeAttackCooldownSystem::operator()(float delta, entt::registry& registry)
{
	int64_t currTimeMillis = OS::get_singleton()->get_ticks_msec();
	auto view = registry.view<AttackPressedTag, CurrentWeaponMeleeTag, MeleeAttackComponent>();
	view.each([&registry, currTimeMillis](entt::entity entity, MeleeAttackComponent& attackComp)
	{
		if (attackComp.prevHitTime + utils::SecondsToMillis(attackComp.GetCurrentHit().attackTime) <= currTimeMillis)
			attackComp.prevHitTime = currTimeMillis;
		else
			registry.remove<AttackPressedTag>(entity); //TODO_melee: accumulate next input for continuing combo
	});
}