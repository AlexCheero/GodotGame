#include "WeaponChooseRSystem.h"

#include "../Components/InputComponents.h"
#include "../Components/AttackComponents.h"

namespace //private
{
	void OnMeleeChoose(entt::registry& registry, entt::entity entity)
	{
		if (!registry.has<CurrentWeaponMeleeTag>(entity))
			registry.assign<CurrentWeaponMeleeTag>(entity);
	}

	void OnRangedChoose(entt::registry& registry, entt::entity entity)
	{
		if (!registry.has<CurrentWeaponRangedTag>(entity))
			registry.assign<CurrentWeaponRangedTag>(entity);
	}

	void OnThrowableChoose(entt::registry& registry, entt::entity entity)
	{
		if (!registry.has<CurrentWeaponThrowableTag>(entity))
			registry.assign<CurrentWeaponThrowableTag>(entity);
	}

	void OnMeleeAttackComponentAssign(entt::registry& registry, entt::entity entity)
	{
		registry.assign_or_replace<CurrentWeaponMeleeTag>(entity);
	}

	void OnRangedAttackComponentAssign(entt::registry& registry, entt::entity entity)
	{
		registry.assign_or_replace<CurrentWeaponRangedTag>(entity);
	}

	void OnThrowableAttackComponentAssign(entt::registry& registry, entt::entity entity)
	{
		registry.assign_or_replace<CurrentWeaponThrowableTag>(entity);
	}

	void OnMeleeTagConstruct(entt::registry& registry, entt::entity entity)
	{
		ASSERT(registry.has<MeleeAttackComponent>(entity), "registry has no MeleeAttackComponent");
		registry.remove_if_exists<CurrentWeaponRangedTag>(entity);
		registry.remove_if_exists<CurrentWeaponThrowableTag>(entity);
	}

	void OnRangedTagConstruct(entt::registry& registry, entt::entity entity)
	{
		ASSERT(registry.has<RangedAttackComponent>(entity), "registry has no RangedAttackComponent");
		registry.remove_if_exists<CurrentWeaponMeleeTag>(entity);
		registry.remove_if_exists<CurrentWeaponThrowableTag>(entity);
	}

	void OnThrowableTagConstruct(entt::registry& registry, entt::entity entity)
	{
		ASSERT(registry.has<ThrowableAttackComponent>(entity), "registry has no ThrowableAttackComponent");
		registry.remove_if_exists<CurrentWeaponMeleeTag>(entity);
		registry.remove_if_exists<CurrentWeaponRangedTag>(entity);
	}
}

void godot::WeaponChooseRSystem::Init(entt::registry& registry)
{
	registry.on_construct<ChooseMeleePressedTag>().connect<&OnMeleeChoose>();
	registry.on_construct<ChooseRangedPressedTag>().connect<&OnRangedChoose>();
	registry.on_construct<ChooseThrowablePressedTag>().connect<&OnThrowableChoose>();

	//<changing current weapon tag
	registry.on_construct<MeleeAttackComponent>().connect<&OnMeleeAttackComponentAssign>();
	registry.on_replace<MeleeAttackComponent>().connect<&OnMeleeAttackComponentAssign>();
	registry.on_construct<RangedAttackComponent>().connect<&OnRangedAttackComponentAssign>();
	registry.on_replace<RangedAttackComponent>().connect<&OnRangedAttackComponentAssign>();
	registry.on_construct<ThrowableAttackComponent>().connect<&OnThrowableAttackComponentAssign>();
	registry.on_replace<ThrowableAttackComponent>().connect<&OnThrowableAttackComponentAssign>();
	//changing current weapon tag>

	//<removing other weapon tags
	registry.on_construct<CurrentWeaponMeleeTag>().connect<&OnMeleeTagConstruct>();
	registry.on_construct<CurrentWeaponRangedTag>().connect<&OnRangedTagConstruct>();
	registry.on_construct<CurrentWeaponThrowableTag>().connect<&OnThrowableTagConstruct>();
	//removing other weapon tags>
}