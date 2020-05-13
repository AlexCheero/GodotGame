#include "IncrementComboRSystem.h"

#include "../Components/AttackComponents.h"
#include "../Components/InputComponents.h"

namespace //private
{
	void OnComboIncrement(entt::registry& registry, entt::entity entity)
	{
		ASSERT(registry.has<MeleeAttackComponent>(entity), "entity has no MeleeAttackComponent");

		MeleeAttackComponent& attackComp = registry.get<MeleeAttackComponent>(entity);

		int prevIdx = attackComp.hitIdx;

		attackComp.hitIdx++;
		if (attackComp.hitIdx > attackComp.hits.size() - 1)
			attackComp.hitIdx = 0;
	}
}

void godot::IncrementComboRSystem::Init(entt::registry& registry)
{
	//TODO0: combo doesnt increment if attack pressed before anim stops playing
	registry.on_destroy<AttackAnimPlayingComponent>().connect<&OnComboIncrement>();
}