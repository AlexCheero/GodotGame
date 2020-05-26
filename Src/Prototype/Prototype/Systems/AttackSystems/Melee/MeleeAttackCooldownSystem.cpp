#include "MeleeAttackCooldownSystem.h"

#include <OS.hpp>

#include "../../../Components/AttackComponents.h"
#include "../../../Components/InputComponents.h"

//TODO: move to config.cfg and tweak value
#define MILLIS_TO_KEEP_COMBO 500

void godot::MeleeAttackCooldownSystem::Tick(float delta, entt::registry& registry)
{
	int64_t currTimeMillis = OS::get_singleton()->get_ticks_msec();

	auto bufferedView = registry.view<MeleeAttackBuffered, CurrentWeaponMeleeTag, MeleeAttackComponent>();
	bufferedView.each([&registry, currTimeMillis](entt::entity entity, MeleeAttackComponent& attackComp)
	{
		int64_t attackTimeMillis = utils::SecondsToMillis(attackComp.GetCurrentHit().attackTime);
		int64_t millisSinceLastHit = currTimeMillis - attackComp.prevHitTime;
		if (millisSinceLastHit >= attackTimeMillis)
		{
			attackComp.prevHitTime = currTimeMillis;

			registry.remove<MeleeAttackBuffered>(entity);
			ASSERT(!registry.has<MeleeAttackEvent>(entity), "MeleeAttackEvent didn't cleared properly");
			registry.emplace<MeleeAttackEvent>(entity);
		}
	});

	//TODO_asap: try to use only AttackPressedTag and remove MeleeAttackEvent
	auto inputView = registry.view<AttackPressedTag, CurrentWeaponMeleeTag, MeleeAttackComponent>(entt::exclude<MeleeAttackBuffered>);
	inputView.each([&registry, currTimeMillis](entt::entity entity, MeleeAttackComponent& attackComp)
	{
		int64_t attackTimeMillis = utils::SecondsToMillis(attackComp.GetCurrentHit().attackTime);
		//TODO: use interval between hits (not including attack time) instead of this assert or use this as feature to implement non combobale hits
		ASSERT(attackTimeMillis < MeleeAttackComponent::maxComboIntervalMillis, "attack time is smaller than max combo interval");
		int64_t millisSinceLastHit = currTimeMillis - attackComp.prevHitTime;
		if (millisSinceLastHit >= attackTimeMillis)
		{
			attackComp.prevHitTime = currTimeMillis;
			ASSERT(!registry.has<MeleeAttackEvent>(entity), "MeleeAttackEvent didn't cleared properly");
			registry.emplace<MeleeAttackEvent>(entity);
		}
		else if (millisSinceLastHit >= MILLIS_TO_KEEP_COMBO && millisSinceLastHit < MeleeAttackComponent::maxComboIntervalMillis)
		{
			registry.emplace<MeleeAttackBuffered>(entity);
		}
	});
}