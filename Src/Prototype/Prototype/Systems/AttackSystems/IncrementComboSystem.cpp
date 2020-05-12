#include "IncrementComboSystem.h"

#include "../../Components/AttackComponents.h"

void godot::IncrementComboSystem::operator()(float delta, entt::registry& registry)
{
	auto incrementComboView = registry.view<IncrementComboTag, MeleeAttackComponent>();
	incrementComboView.less([](MeleeAttackComponent& attackComp)
	{
		attackComp.hitIdx++;
		if (attackComp.hitIdx > attackComp.hits.size() - 1)
			attackComp.hitIdx = 0;
	});
	registry.remove<IncrementComboTag>(incrementComboView.begin(), incrementComboView.end());
}