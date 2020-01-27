#include "WeaponChooseSystem.h"

#include "../Components/InputComponents.h"
#include "../Components/AttackComponents.h"

void godot::WeaponChooseSystem::OnMeleeTagConstruct(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<entt::tag<CurrentWeaponCastTag> >(entity);
	registry.remove_if_exists<entt::tag<CurrentWeaponThrowableTag> >(entity);
}

void godot::WeaponChooseSystem::OnCastTagConstruct(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<entt::tag<CurrentWeaponMeleeTag> >(entity);
	registry.remove_if_exists<entt::tag<CurrentWeaponThrowableTag> >(entity);
}

void godot::WeaponChooseSystem::OnThrowableTagConstruct(entt::registry& registry, entt::entity entity)
{
	registry.remove_if_exists<entt::tag<CurrentWeaponMeleeTag> >(entity);
	registry.remove_if_exists<entt::tag<CurrentWeaponCastTag> >(entity);
}

//TODO: don't update this and similar systems all the time
void godot::WeaponChooseSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<InputComponent>();
	view.each([&registry](entt::entity entity, InputComponent input)
	{
		//TODO: it shouldn't take non existent weapon
		if (input.Test(EInput::ChooseMelee) && !registry.has<entt::tag<CurrentWeaponMeleeTag> >(entity)
			&& registry.has<MeleeAttackComponent>(entity))
		{
			registry.assign<entt::tag<CurrentWeaponMeleeTag> >(entity);
		}
		if (input.Test(EInput::ChooseRanged) && !registry.has<entt::tag<CurrentWeaponCastTag> >(entity)
			&& registry.has<CastAttackComponent>(entity))
		{
			registry.assign<entt::tag<CurrentWeaponCastTag> >(entity);
		}
		if (input.Test(EInput::ChooseThrowable) && !registry.has<entt::tag<CurrentWeaponThrowableTag> >(entity)
			&& registry.has<ThrowableAttackComponent>(entity))
		{
			registry.assign<entt::tag<CurrentWeaponThrowableTag> >(entity);
		}
	});
}