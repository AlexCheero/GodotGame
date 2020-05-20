#include "ComboDropSystem.h"

#include <OS.hpp>

#include "../../../Components/AttackComponents.h"

void godot::ComboDropSystem::OnMeleeHide(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<MeleeAttackComponent>(entity), "entity has no MeleeAttackComponent");
	registry.get<MeleeAttackComponent>(entity).hitIdx = 0;
}

void godot::ComboDropSystem::Init(entt::registry& registry)
{
	registry.on_destroy<CurrentWeaponMeleeTag>().connect<&ComboDropSystem::OnMeleeHide>();
}

void godot::ComboDropSystem::Tick(float delta, entt::registry& registry)
{
	int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
	auto view = registry.view<MeleeAttackComponent>();
	view.each([currTimeMillis](MeleeAttackComponent& attackComp)
	{
		int64_t millisSinceLastHit = currTimeMillis - attackComp.prevHitTime;
		if (millisSinceLastHit > MeleeAttackComponent::maxComboIntervalMillis)
			attackComp.hitIdx = 0;
	});
}