#include "CastAttackSystem.h"

#include <Spatial.hpp>
#include <OS.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Components/NodeComponents/EntityHolderNodeComponent.h"

#include "../../Utils/Utils.h"

void godot::CastAttackSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<CastAttackComponent, InputComponent, Spatial*>(ExcludeDead);
	view.each([&registry, this](CastAttackComponent& attackComp, InputComponent input, Spatial* pAttackerSpatial)
	{
		if (!CanAttack(input, attackComp.attackTime, attackComp.prevHitTime))
			return;

		Godot::print("Bang!");

		Object* pObj = utils::CastFromSpatial(pAttackerSpatial, attackComp.distance);
		if (!pObj)
			return;

		entt::entity enemyEntity = Object::cast_to<EntityHolderNodeComponent>(pObj)->GetEntity();

		HealthComponent& enemyHealthComp = registry.get<HealthComponent>(enemyEntity);
		enemyHealthComp.hp -= attackComp.damage;

		if (enemyHealthComp.hp <= 0)
		{
			Godot::print("Cast kill!");
			enemyHealthComp.hp = 0;
			registry.assign<entt::tag<DeadTag> >(enemyEntity);
		}
		else
			Godot::print("Cast hit!");
	});
}