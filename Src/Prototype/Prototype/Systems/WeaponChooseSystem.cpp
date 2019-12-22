#include "WeaponChooseSystem.h"

#include "../Components/InputComponents.h"
#include "../Components/AttackComponents.h"

void godot::WeaponChooseSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<InputComponent, WeaponHolderComponent>().each([&registry](entt::entity entity, InputComponent input, WeaponHolderComponent weapons)
	{
		if (input.Test(EInput::ChooseMelee) && !registry.has<MelleAttackComponent>(entity))
		{
			registry.assign<MelleAttackComponent>(entity, weapons.melee);
			registry.reset<CastAttackComponent>(entity);
		}
		if (input.Test(EInput::ChooseRanged) && !registry.has<CastAttackComponent>(entity))
		{
			registry.reset<MelleAttackComponent>(entity);
			registry.assign<CastAttackComponent>(entity, weapons.ranged);
		}
	});
}