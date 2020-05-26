#include "MeleeAttackCooldownSystem.h"

#include <OS.hpp>

#include "../../../Components/AttackComponents.h"
#include "../../../Components/InputComponents.h"

//TODO: move to config and tweak value
#define MILLIS_TO_KEEP_COMBO 500

void godot::MeleeAttackCooldownSystem::Tick(float delta, entt::registry& registry)
{
	int64_t currTimeMillis = OS::get_singleton()->get_ticks_msec();

	auto bufferedView = registry.view<MeleeAttackBuffered, CurrentWeaponMeleeTag, MeleeAttackComponent>();
	bufferedView.each([&registry, currTimeMillis](entt::entity entity, MeleeAttackComponent& attackComp)
	{
		int64_t attackTimeMillis = utils::SecondsToMillis(attackComp.GetCurrentHit().attackTime);
		if (attackComp.prevHitTime + attackTimeMillis <= currTimeMillis)
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
		//do not use timePassedSinceLastAttack because attackComp.prevHitTime is initialy -std::numeric_limits<int64_t>::max()
		//and this cause type underflow
		//int64_t timePassedSinceLastAttack = currTimeMillis - attackComp.prevHitTime;
		//if (timePassedSinceLastAttack >= attackTimeMillis)

		int64_t attackTimeMillis = utils::SecondsToMillis(attackComp.GetCurrentHit().attackTime);
		if (attackComp.prevHitTime + attackTimeMillis <= currTimeMillis)
		{
			attackComp.prevHitTime = currTimeMillis;
			ASSERT(!registry.has<MeleeAttackEvent>(entity), "MeleeAttackEvent didn't cleared properly");
			registry.emplace<MeleeAttackEvent>(entity);
		}
		else if (attackComp.prevHitTime + MILLIS_TO_KEEP_COMBO <= currTimeMillis) //TODO_asap: keep only if incrementing combo
		{
			registry.emplace<MeleeAttackBuffered>(entity);
		}
	});
}