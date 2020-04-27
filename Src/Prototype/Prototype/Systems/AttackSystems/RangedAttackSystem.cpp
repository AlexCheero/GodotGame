#include "RangedAttackSystem.h"

#include <Spatial.hpp>
#include <OS.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "../../Nodes/EntityView.h"

#include "../../Components/SimpleComponents.h"
#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"

#include "../../Utils/Utils.h"

void godot::RangedAttackSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<CurrentWeaponRangedTag, RangedAttackComponent, InputComponent, Spatial*>(ExcludeDead);
	view.less([&registry, this](entt::entity entity, RangedAttackComponent& attackComp, InputComponent input, Spatial* pAttackerSpatial)
	{
		if (!CanAttack(input, attackComp.attackTime, attackComp.prevHitTime))
			return;

		attackComp.ammoCount--;
		ASSERT(attackComp.ammoCount >= 0, "negative ammo count");
		//TODO: instantly melee hits or don't changes weapon at all, after throwing weapon. fix this after refactoring whole input system
		//		input system already refactored
		if (attackComp.ammoCount == 0 /*&& throw on out of ammo*/)
			registry.remove<RangedAttackComponent>(entity);

		Godot::print("Bang!");

		Vector3 castDirection = pAttackerSpatial->get_global_transform().get_basis().z;
		Object* pObj = utils::CastFromSpatial(pAttackerSpatial, castDirection, attackComp.distance, utils::GetDamageableMask());
		if (!pObj)
			return;

		Node* pNode = Object::cast_to<Node>(pObj);
		ASSERT(pNode != nullptr, "hitted object is not node");
		if (!pNode->has_node("EntityView"))
			return;

		EntityView* pEntityView = Object::cast_to<EntityView>(pNode->get_node("EntityView"));
		if (!pEntityView)
			return;

		//TODO: check teammate if friendly fire is off

		entt::entity enemyEntity = pEntityView->GetEntity();

		HealthComponent& enemyHealthComp = registry.get<HealthComponent>(enemyEntity);
		enemyHealthComp.hp -= attackComp.damage;

		Godot::print("ranged hit");
	});
}