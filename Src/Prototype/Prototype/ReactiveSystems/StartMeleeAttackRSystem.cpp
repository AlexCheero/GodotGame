#include "StartMeleeAttackRSystem.h"

#include <OS.hpp>

#include "../Components/AttackComponents.h"
#include "../Components/InputComponents.h"

namespace //private
{
	//TODO0: convert to attack cooldown check. probably remove utils::Expired after converting
	void OnInputPressed(entt::registry& registry, entt::entity entity)
	{
		if (!registry.has<CurrentWeaponMeleeTag>(entity) || registry.any<AttackAnimPlayingComponent>(entity))
			return;

		ASSERT(registry.has<MeleeAttackComponent>(entity), "entity has no MeleeAttackComponent");

		int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
		MeleeAttackComponent& attackComp = registry.get<MeleeAttackComponent>(entity);
		if (attackComp.prevHitTimeMillis + utils::SecondsToMillis(attackComp.GetCurrentHit().attackTime) <= currTimeMillis)
		{
			int64_t millisSinceLastHit = currTimeMillis - attackComp.prevHitTimeMillis;
			attackComp.prevHitTimeMillis = currTimeMillis;
		}
	}
}

void godot::StartMeleeAttackRSystem::Init(entt::registry& registry)
{
	registry.on_construct<AttackPressedTag>().connect<&OnInputPressed>();
}
