#include "WeaponChooseSystem.h"

#include "../Components/InputComponents.h"
#include "../Components/AttackComponents.h"

void godot::WeaponChooseSystem::OnMeleeAttackComponentAssign(entt::registry& registry, entt::entity entity)
{
	registry.assign_or_replace<CurrentWeaponMeleeTag>(entity);
}

void godot::WeaponChooseSystem::OnRangedAttackComponentAssign(entt::registry& registry, entt::entity entity)
{
	registry.assign_or_replace<CurrentWeaponRangedTag>(entity);
}

void godot::WeaponChooseSystem::OnThrowableAttackComponentAssign(entt::registry& registry, entt::entity entity)
{
	registry.assign_or_replace<CurrentWeaponThrowableTag>(entity);
}

void godot::WeaponChooseSystem::OnMeleeTagConstruct(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<MeleeAttackComponent>(entity), "registry has no MeleeAttackComponent");
	registry.remove_if_exists<CurrentWeaponRangedTag>(entity);
	registry.remove_if_exists<CurrentWeaponThrowableTag>(entity);
}

void godot::WeaponChooseSystem::OnRangedTagConstruct(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<RangedAttackComponent>(entity), "registry has no RangedAttackComponent");
	registry.remove_if_exists<CurrentWeaponMeleeTag>(entity);
	registry.remove_if_exists<CurrentWeaponThrowableTag>(entity);
}

void godot::WeaponChooseSystem::OnThrowableTagConstruct(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<ThrowableAttackComponent>(entity), "registry has no ThrowableAttackComponent");
	registry.remove_if_exists<CurrentWeaponMeleeTag>(entity);
	registry.remove_if_exists<CurrentWeaponRangedTag>(entity);
}

godot::WeaponChooseSystem::WeaponChooseSystem(entt::registry& registry)
{
	//<changing current weapon tag
	registry.on_construct<MeleeAttackComponent>().connect<&WeaponChooseSystem::OnMeleeAttackComponentAssign>(this);
	registry.on_replace<MeleeAttackComponent>().connect<&WeaponChooseSystem::OnMeleeAttackComponentAssign>(this);
	registry.on_construct<RangedAttackComponent>().connect<&WeaponChooseSystem::OnRangedAttackComponentAssign>(this);
	registry.on_replace<RangedAttackComponent>().connect<&WeaponChooseSystem::OnRangedAttackComponentAssign>(this);
	registry.on_construct<ThrowableAttackComponent>().connect<&WeaponChooseSystem::OnThrowableAttackComponentAssign>(this);
	registry.on_replace<ThrowableAttackComponent>().connect<&WeaponChooseSystem::OnThrowableAttackComponentAssign>(this);
	//changing current weapon tag>

	//<removing other weapon tags
	registry.on_construct<CurrentWeaponMeleeTag>().connect<&WeaponChooseSystem::OnMeleeTagConstruct>(this);
	registry.on_construct<CurrentWeaponRangedTag>().connect<&WeaponChooseSystem::OnRangedTagConstruct>(this);
	registry.on_construct<CurrentWeaponThrowableTag>().connect<&WeaponChooseSystem::OnThrowableTagConstruct>(this);
	//removing other weapon tags>
}

//TODO: don't update this and similar systems all the time
void godot::WeaponChooseSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<InputComponent>();
	view.each([&registry](entt::entity entity, InputComponent input)
	{
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