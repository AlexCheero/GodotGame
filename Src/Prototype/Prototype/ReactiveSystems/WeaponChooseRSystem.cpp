#include "WeaponChooseRSystem.h"

#include "../Components/InputComponents.h"
#include "../Components/AttackComponents.h"

void godot::WeaponChooseRSystem::OnMeleeChoose(entt::registry& registry, entt::entity entity)
{
	if (!registry.has<CurrentWeaponMeleeTag>(entity))
		registry.emplace<CurrentWeaponMeleeTag>(entity);
}

void godot::WeaponChooseRSystem::OnRangedChoose(entt::registry& registry, entt::entity entity)
{
	if (!registry.has<CurrentWeaponRangedTag>(entity))
		registry.emplace<CurrentWeaponRangedTag>(entity);
}

void godot::WeaponChooseRSystem::OnThrowableChoose(entt::registry& registry, entt::entity entity)
{
	if (!registry.has<CurrentWeaponThrowableTag>(entity))
		registry.emplace<CurrentWeaponThrowableTag>(entity);
}

void godot::WeaponChooseRSystem::OnGrenadeChoose(entt::registry& registry, entt::entity entity)
{
	if (!registry.has<CurrentWeaponGrenadeTag>(entity))
		registry.emplace<CurrentWeaponGrenadeTag>(entity);
}

void godot::WeaponChooseRSystem::OnMeleeAttackComponentAssign(entt::registry& registry, entt::entity entity)
{
	registry.emplace_or_replace<CurrentWeaponMeleeTag>(entity);
}

void godot::WeaponChooseRSystem::SwitchToMeleeOnWeaponThrow(entt::registry& registry, entt::entity entity)
{
	registry.emplace_or_replace<CurrentWeaponMeleeTag>(entity);
	registry.remove_if_exists<AttackPressedTag>(entity);
}

void godot::WeaponChooseRSystem::OnRangedAttackComponentAssign(entt::registry& registry, entt::entity entity)
{
	registry.emplace_or_replace<CurrentWeaponRangedTag>(entity);
}

void godot::WeaponChooseRSystem::OnThrowableAttackComponentAssign(entt::registry& registry, entt::entity entity)
{
	registry.emplace_or_replace<CurrentWeaponThrowableTag>(entity);
}

void godot::WeaponChooseRSystem::OnGrenadeAttackComponentAssign(entt::registry& registry, entt::entity entity)
{
	registry.emplace_or_replace<CurrentWeaponGrenadeTag>(entity);
}

void godot::WeaponChooseRSystem::OnMeleeTagConstruct(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<MeleeAttackComponent>(entity), "registry has no MeleeAttackComponent");
	registry.remove_if_exists<CurrentWeaponRangedTag>(entity);
	registry.remove_if_exists<CurrentWeaponThrowableTag>(entity);
	registry.remove_if_exists<CurrentWeaponGrenadeTag>(entity);
}

void godot::WeaponChooseRSystem::OnRangedTagConstruct(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<RangedAttackComponent>(entity), "registry has no RangedAttackComponent");
	registry.remove_if_exists<CurrentWeaponMeleeTag>(entity);
	registry.remove_if_exists<CurrentWeaponThrowableTag>(entity);
	registry.remove_if_exists<CurrentWeaponGrenadeTag>(entity);
}

void godot::WeaponChooseRSystem::OnThrowableTagConstruct(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<ThrowableAttackComponent>(entity), "registry has no ThrowableAttackComponent");
	registry.remove_if_exists<CurrentWeaponMeleeTag>(entity);
	registry.remove_if_exists<CurrentWeaponRangedTag>(entity);
	registry.remove_if_exists<CurrentWeaponGrenadeTag>(entity);
}

void godot::WeaponChooseRSystem::OnGrenadeTagConstruct(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<GrenadeAttackComponent>(entity), "registry has no ThrowableAttackComponent");
	registry.remove_if_exists<CurrentWeaponMeleeTag>(entity);
	registry.remove_if_exists<CurrentWeaponRangedTag>(entity);
	registry.remove_if_exists<CurrentWeaponThrowableTag>(entity);
}

void godot::WeaponChooseRSystem::Init(entt::registry& registry)
{
	registry.on_construct<ChooseMeleePressedTag>().connect<&OnMeleeChoose>();
	registry.on_construct<ChooseRangedPressedTag>().connect<&OnRangedChoose>();
	registry.on_construct<ChooseThrowablePressedTag>().connect<&OnThrowableChoose>();
	registry.on_construct<ChooseGrenadePressedTag>().connect<&OnGrenadeChoose>();

	//<changing current weapon tag
	registry.on_construct<MeleeAttackComponent>().connect<&OnMeleeAttackComponentAssign>();
	registry.on_update<MeleeAttackComponent>().connect<&OnMeleeAttackComponentAssign>();
	registry.on_construct<RangedAttackComponent>().connect<&OnRangedAttackComponentAssign>();
	registry.on_update<RangedAttackComponent>().connect<&OnRangedAttackComponentAssign>();
	registry.on_destroy<RangedAttackComponent>().connect<&SwitchToMeleeOnWeaponThrow>();
	registry.on_construct<ThrowableAttackComponent>().connect<&OnThrowableAttackComponentAssign>();
	registry.on_update<ThrowableAttackComponent>().connect<&OnThrowableAttackComponentAssign>();
	registry.on_destroy<ThrowableAttackComponent>().connect<&SwitchToMeleeOnWeaponThrow>();
	registry.on_construct<GrenadeAttackComponent>().connect<&OnGrenadeAttackComponentAssign>();
	registry.on_update<GrenadeAttackComponent>().connect<&OnGrenadeAttackComponentAssign>();
	registry.on_destroy<GrenadeAttackComponent>().connect<&SwitchToMeleeOnWeaponThrow>();
	//changing current weapon tag>

	//<removing other weapon tags
	registry.on_construct<CurrentWeaponMeleeTag>().connect<&OnMeleeTagConstruct>();
	registry.on_construct<CurrentWeaponRangedTag>().connect<&OnRangedTagConstruct>();
	registry.on_construct<CurrentWeaponThrowableTag>().connect<&OnThrowableTagConstruct>();
	registry.on_construct<CurrentWeaponGrenadeTag>().connect<&OnGrenadeTagConstruct>();
	//removing other weapon tags>
}