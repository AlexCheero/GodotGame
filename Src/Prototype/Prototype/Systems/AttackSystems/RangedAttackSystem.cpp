#include "RangedAttackSystem.h"

#include <Spatial.hpp>
#include <OS.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Components/NodeComponents/EntityHolderNodeComponent.h"

#include "../../Utils/Utils.h"

void godot::RangedAttackSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<entt::tag<CurrentWeaponRangedTag>, RangedAttackComponent, InputComponent, Spatial*>(ExcludeDead);
	view.less([&registry, this](entt::entity entity, RangedAttackComponent& attackComp, InputComponent input, Spatial* pAttackerSpatial)
	{
		if (!CanAttack(input, attackComp.attackTime, attackComp.prevHitTime))
			return;

		attackComp.ammoCount--;
		ASSERT(attackComp.ammoCount >= 0, "negative ammo count");
		//TODO: instantly melee hits or don't changes weapon at all, after throwing weapon. fix this after refactoring whole input system
		if (attackComp.ammoCount == 0 /*&& throw on out of ammo*/)
			registry.remove<RangedAttackComponent>(entity);

		Godot::print("Bang!");

		Object* pObj = utils::CastFromSpatial(pAttackerSpatial, attackComp.distance);
		if (!pObj)
			return;

		EntityHolderNodeComponent* entityHolder = Object::cast_to<EntityHolderNodeComponent>(pObj);
		if (!entityHolder)
			return;

		entt::entity enemyEntity = entityHolder->GetEntity();

		HealthComponent& enemyHealthComp = registry.get<HealthComponent>(enemyEntity);
		enemyHealthComp.hp -= attackComp.damage;

		if (enemyHealthComp.hp <= 0)
		{
			Godot::print("Ranged kill!");
			enemyHealthComp.hp = 0;
			registry.assign<entt::tag<DeadTag> >(enemyEntity);
		}
		else
			Godot::print("Ranged hit!");
	});
}