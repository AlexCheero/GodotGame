#include "IncrementComboSystem.h"

#include "../../../Components/AttackComponents.h"
#include "../../../Components/InputComponents.h"

void godot::IncrementComboSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<AttackPressedTag, MeleeAttackComponent>();
	view.less([](MeleeAttackComponent& attackComp)
	{
		int prevIdx = attackComp.hitIdx;

		//TODO: drop combo on switching from melee here or in ComboDropSystem
		attackComp.hitIdx++;
		if (attackComp.hitIdx > attackComp.hits.size() - 1)
			attackComp.hitIdx = 0;
	});
}