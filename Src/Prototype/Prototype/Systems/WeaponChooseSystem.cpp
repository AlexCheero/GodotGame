#include "WeaponChooseSystem.h"

#include "../Components/InputComponents.h"
#include "../Components/AttackComponents.h"

void godot::WeaponChooseSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<InputComponent, WeaponHolderComponent>();
	view.each([&registry](entt::entity entity, InputComponent input, WeaponHolderComponent weapons)
	{
		if (input.Test(EInput::ChooseMelee) && !registry.has<MeleeAttackComponent>(entity))
		{
			registry.assign<MeleeAttackComponent>(entity, weapons.melee);
			registry.reset<CastAttackComponent>(entity);
			registry.reset<ThrowableAttackComponent>(entity);
		}
		if (input.Test(EInput::ChooseRanged) && !registry.has<CastAttackComponent>(entity))
		{
			registry.reset<MeleeAttackComponent>(entity);
			registry.assign<CastAttackComponent>(entity, weapons.ranged);
			registry.reset<ThrowableAttackComponent>(entity);
		}
		if (input.Test(EInput::ChooseThrowable) && !registry.has<ThrowableAttackComponent>(entity))
		{
			Godot::print("ChooseThrowable");
			registry.reset<MeleeAttackComponent>(entity);
			registry.reset<CastAttackComponent>(entity);
			registry.assign<ThrowableAttackComponent>(entity, weapons.throwable);
		}
	});
}