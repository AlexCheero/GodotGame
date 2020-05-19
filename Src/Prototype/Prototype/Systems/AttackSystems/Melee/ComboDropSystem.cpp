#include "ComboDropSystem.h"

#include <OS.hpp>

#include "../../../Components/AttackComponents.h"

void godot::ComboDropSystem::OnMeleeHide(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<MeleeAttackComponent>(entity), "entity has no MeleeAttackComponent");
	registry.get<MeleeAttackComponent>(entity).hitIdx = 0;
}

godot::ComboDropSystem::ComboDropSystem(entt::registry& registry)
{
	registry.on_destroy<CurrentWeaponMeleeTag>().connect<&ComboDropSystem::OnMeleeHide>(this);
}

void godot::ComboDropSystem::operator()(float delta, entt::registry& registry)
{
	int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
	auto view = registry.view<MeleeAttackComponent, PrevAttackTime>();
	view.each([currTimeMillis](MeleeAttackComponent& attackComp, PrevAttackTime prevAttack)
	{
		int64_t millisSinceLastHit = currTimeMillis - prevAttack.millis;
		if (millisSinceLastHit > MeleeAttackComponent::maxComboIntervalMillis)
			attackComp.hitIdx = 0;
	});
}