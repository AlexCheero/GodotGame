#include "IncrementComboSystem.h"

#include "../../../Components/AttackComponents.h"
#include "../../../Components/InputComponents.h"

void godot::IncrementComboSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<AttackPressedTag, CurrentWeaponMeleeTag, MeleeAttackComponent>();
	view.each([](MeleeAttackComponent& attackComp)
	{
		int prevIdx = attackComp.hitIdx;

		attackComp.hitIdx++;
		if (attackComp.hitIdx > attackComp.hits.size() - 1)
			attackComp.hitIdx = 0;
	});
}