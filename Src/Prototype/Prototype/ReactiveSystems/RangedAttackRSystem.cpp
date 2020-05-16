#include "RangedAttackRSystem.h"

#include <Spatial.hpp>
#include <OS.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "../Nodes/EntityView.h"

#include "../Components/SimpleComponents.h"
#include "../Components/AttackComponents.h"
#include "../Components/InputComponents.h"

#include "../Utils/Utils.h"

void godot::RangedAttackSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<AttackPressedTag, CurrentWeaponRangedTag, RangedAttackComponent, Spatial*>();
	view.less([&registry](entt::entity entity, RangedAttackComponent& attackComp, Spatial* pAttackerSpatial)
	{
		if (!utils::Expired(attackComp.attackTime, attackComp.prevHitTime))
			return;

		attackComp.ammoCount--;
		ASSERT(attackComp.ammoCount >= 0, "negative ammo count");
		//TODO: instantly melee hits or don't changes weapon at all, after throwing weapon. fix this after refactoring whole input system
		//		input system already refactored
		if (attackComp.ammoCount == 0 /*&& throw on out of ammo*/)
			registry.remove<RangedAttackComponent>(entity);

		godot::Godot::print("Bang!");

		godot::Vector3 castDirection = pAttackerSpatial->get_global_transform().get_basis().z;
		godot::Spatial* pSpatial = utils::CastFromSpatial(pAttackerSpatial, castDirection, attackComp.distance, utils::GetDamageableMask());
		if (!pSpatial)
			return;

		if (!pSpatial->has_node("EntityView"))
			return;

		godot::EntityView* pEntityView = godot::Object::cast_to<godot::EntityView>(pSpatial->get_node("EntityView"));
		if (!pEntityView)
			return;

		//TODO: check teammate if friendly fire is off

		entt::entity enemyEntity = pEntityView->GetEntity();

		HealthComponent& enemyHealthComp = registry.get<HealthComponent>(enemyEntity);
		enemyHealthComp.hp -= attackComp.damage;

		godot::Godot::print("ranged hit");
	});
}