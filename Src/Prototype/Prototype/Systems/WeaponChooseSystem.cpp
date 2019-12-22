#include "WeaponChooseSystem.h"

#include "../Components/InputComponents.h"
#include "../Components/AttackComponents.h"

void godot::WeaponChooseSystem::operator()(float delta, entt::registry& registry)
{
	//TODO: very dirty draft. change asap
	registry.view<InputComponent>().each([&registry](entt::entity entity, InputComponent input)
	{
		if (input.Test(EInput::ChooseMelee) && !registry.has<MelleAttackComponent>(entity))
		{
			registry.assign<MelleAttackComponent>(entity, 4.f, 10.f, 90.f, 0.5f);
			registry.remove<CastAttackComponent>(entity);
		}
		if (input.Test(EInput::ChooseRanged) && !registry.has<CastAttackComponent>(entity))
		{
			registry.remove<MelleAttackComponent>(entity);
			registry.assign<CastAttackComponent>(entity, 40.f, 50.f, 0.5f);
		}
	});
}