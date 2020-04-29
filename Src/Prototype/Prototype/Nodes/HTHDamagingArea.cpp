#include "HTHDamagingArea.h"

#include <SceneTree.hpp>

#include "../ECSWorld.h"
#include "../Utils/Utils.h"
#include "../Components/AIComponents/FSMStateComponents.h"
#include "../Nodes/EntityView.h"
#include "../Components/AIComponents/PatrolComponents.h"

void godot::HTHDamagingArea::_register_methods()
{
	register_method((char*)"_on_Area_body_entered", &HTHDamagingArea::_on_Area_body_entered);
	register_method((char*)"_assign_owner_entity", &HTHDamagingArea::_assign_owner_entity);
	register_method((char*)"_ready", &HTHDamagingArea::_ready);
}

void godot::HTHDamagingArea::_ready()
{
	call_deferred("_assign_owner_entity");
	set_collision_layer(0);
	set_collision_mask(utils::GetDamageableMask());
}

void godot::HTHDamagingArea::_on_Area_body_entered(KinematicBody* pBody)
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
	registry.get<HealthComponent>(hittedEntity).hp -= registry.get<MeleeAttackComponent>(ownerEntity).GetCurrentHit().damage;

	//TODO: make bot fight in hth stuck state, if it has critical hp
	if (registry.has<BotTag>(hittedEntity))
	{
		PatrolmanComponent& patrolManComp = registry.get<PatrolmanComponent>(hittedEntity);
		registry.assign_or_replace<HittedFromComponent>(hittedEntity, get_global_transform().get_origin(), patrolManComp.lookAroundTime);
	}
}

void godot::HTHDamagingArea::_assign_owner_entity()
{
	ASSERT(get_owner()->has_node("../EntityView"), "there is no entity view in hierarchy");
	EntityView* pEntityView = Object::cast_to<EntityView>(get_owner()->get_node("../EntityView"));
	ASSERT(pEntityView != nullptr, "cant cast to EntityView");
	ownerEntity = pEntityView->GetEntity();
	ASSERT(ECSWorld::GetInstance()->GetRegistry().valid(ownerEntity), "HTHDamagingArea invalid holder entity");
}
