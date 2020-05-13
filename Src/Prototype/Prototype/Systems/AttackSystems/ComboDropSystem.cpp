#include "ComboDropSystem.h"

#include <OS.hpp>

#include "../../Components/AttackComponents.h"

void godot::ComboDropSystem::operator()(float delta, entt::registry& registry)
{
	int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
	auto view = registry.view<MeleeAttackComponent>();
	view.each([currTimeMillis](MeleeAttackComponent& attackComp)
	{
		int64_t millisSinceLastHit = currTimeMillis - attackComp.prevHitTimeMillis;
		if (millisSinceLastHit > MeleeAttackComponent::maxComboIntervalMillis /*&& attackComp.hitIdx > 0*/)
		{
			//Godot::print("Combo dropped " + String::num_int64(millisSinceLastHit));
			attackComp.hitIdx = 0;
		}
	});
}