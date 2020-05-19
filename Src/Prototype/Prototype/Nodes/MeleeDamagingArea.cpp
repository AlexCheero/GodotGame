#include "MeleeDamagingArea.h"

#include <SceneTree.hpp>

#include "../ECSWorld.h"
#include "../Utils/Utils.h"
#include "../Components/AIComponents/FSMStateComponents.h"
#include "../Nodes/EntityView.h"
#include "../Components/AIComponents/PatrolComponents.h"

void godot::MeleeDamagingArea::_register_methods()
{
	register_method((char*)"_on_Area_body_entered", &MeleeDamagingArea::_on_Area_body_entered);
	register_method((char*)"_assign_owner_entity", &MeleeDamagingArea::_assign_owner_entity);
	register_method((char*)"_ready", &MeleeDamagingArea::_ready);
}

void godot::MeleeDamagingArea::_ready()
{
	call_deferred("_assign_owner_entity");
	set_collision_layer(0);
	set_collision_mask(utils::GetDamageableMask());
}

void godot::MeleeDamagingArea::_on_Area_body_entered(KinematicBody* pBody)
{
	ASSERT(pBody != nullptr, "kinematic body is null");
	if (!pBody->has_node("EntityView"))
		return;

	EntityView* pEntityView = Object::cast_to<EntityView>(pBody->get_node("EntityView"));
	entt::entity hittedEntity = pEntityView->GetEntity();

	if (hittedEntity == ownerEntity)
		return;

	//TODO: check teammate if friendly fire is off

	entt::registry& registry = ECSWorld::GetInstance()->GetRegistry();
	
	ASSERT(registry.has<HealthComponent>(hittedEntity), "hittedEntity has no HealthComponent");
	ASSERT(registry.has<MeleeAttackComponent>(ownerEntity), "ownerEntity has no MeleeAttackComponent");
	float damage = registry.get<MeleeAttackComponent>(ownerEntity).GetCurrentHit().damage;
	registry.get<HealthComponent>(hittedEntity).hp -= damage;

	if (registry.has<BotTag>(hittedEntity))
	{
		PatrolmanComponent& patrolManComp = registry.get<PatrolmanComponent>(hittedEntity);
		registry.emplace_or_replace<HittedFromComponent>(hittedEntity, get_global_transform().get_origin(), patrolManComp.lookAroundTime);
	}
}

void godot::MeleeDamagingArea::_assign_owner_entity()
{
	ASSERT(get_owner()->has_node("../EntityView"), "there is no entity view in hierarchy");
	EntityView* pEntityView = Object::cast_to<EntityView>(get_owner()->get_node("../EntityView"));
	ASSERT(pEntityView != nullptr, "cant cast to EntityView");
	ownerEntity = pEntityView->GetEntity();
	ASSERT(ECSWorld::GetInstance()->GetRegistry().valid(ownerEntity), "MeleeDamagingArea invalid holder entity");
}
