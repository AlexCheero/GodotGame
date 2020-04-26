#include "WeaponChooseSystem.h"

#include "../Components/InputComponents.h"
#include "../Components/AttackComponents.h"

void godot::WeaponChooseSystem::OnMeleeTagConstruct(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<CurrentWeaponRangedTag>(entity);
	registry.remove_if_exists<CurrentWeaponThrowableTag>(entity);
}

void godot::WeaponChooseSystem::OnRangedTagConstruct(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<CurrentWeaponMeleeTag>(entity);
	registry.remove_if_exists<CurrentWeaponThrowableTag>(entity);
}

void godot::WeaponChooseSystem::OnThrowableTagConstruct(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<CurrentWeaponMeleeTag>(entity);
	registry.remove_if_exists<CurrentWeaponRangedTag>(entity);
}

godot::WeaponChooseSystem::WeaponChooseSystem(entt::registry& registry)
{
	registry.on_construct<CurrentWeaponMeleeTag>().connect<&WeaponChooseSystem::OnMeleeTagConstruct>(this);
	registry.on_construct<CurrentWeaponRangedTag>().connect<&WeaponChooseSystem::OnRangedTagConstruct>(this);
	registry.on_construct<CurrentWeaponThrowableTag>().connect<&WeaponChooseSystem::OnThrowableTagConstruct>(this);
}

//TODO: don't update this and similar systems all the time
void godot::WeaponChooseSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<InputComponent>();
	view.each([&registry](entt::entity entity, InputComponent input)
	{
		//TODO: it shouldn't take non existent weapon
		if (input.Test(EInput::ChooseMelee) && !registry.has<CurrentWeaponMeleeTag>(entity)
			&& registry.has<MeleeAttackComponent>(entity))
		{
			registry.assign<CurrentWeaponMeleeTag>(entity);
		}
		if (input.Test(EInput::ChooseRanged) && !registry.has<CurrentWeaponRangedTag>(entity)
			&& registry.has<RangedAttackComponent>(entity))
		{
			registry.assign<CurrentWeaponRangedTag>(entity);
		}
		if (input.Test(EInput::ChooseThrowable) && !registry.has<CurrentWeaponThrowableTag>(entity)
			&& registry.has<ThrowableAttackComponent>(entity))
		{
			registry.assign<CurrentWeaponThrowableTag>(entity);
		}
	});
}