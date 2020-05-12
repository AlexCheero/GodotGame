#include "StartMeleeAttackRSystem.h"

#include <OS.hpp>

#include "../Components/AttackComponents.h"
#include "../Components/InputComponents.h"

namespace //private
{
	void OnInputPressed(entt::registry& registry, entt::entity entity)
	{
		//TODO0: try to abstract
		if (!registry.has<CurrentWeaponMeleeTag>(entity) || registry.has<AttackAnimPlayingComponent>(entity))
			return;

		int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();

		ASSERT(registry.has<MeleeAttackComponent>(entity), "entity has no MeleeAttackComponent");

		MeleeAttackComponent& attackComp = registry.get<MeleeAttackComponent>(entity);
		if (attackComp.prevHitTimeMillis + utils::SecondsToMillis(attackComp.GetCurrentHit().attackTime) > currTimeMillis)
		{
			registry.remove<AttackPressedTag>(entity);
			return;
		}

		int64_t millisSinceLastHit = currTimeMillis - attackComp.prevHitTimeMillis;
		attackComp.prevHitTimeMillis = currTimeMillis;

		//TODO0: increment bug, when attack is pressed before animation stops playing
		//       probably this because of wrong EndAttackAnimSystem update
		if (millisSinceLastHit <= MeleeAttackComponent::maxComboIntervalMillis)
			registry.assign<IncrementComboTag>(entity);
	}
}

void godot::StartMeleeAttackRSystem::Init(entt::registry& registry)
{
	registry.on_construct<AttackPressedTag>().connect<&OnInputPressed>();
}
