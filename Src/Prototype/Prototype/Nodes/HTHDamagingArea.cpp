#include "HTHDamagingArea.h"

#include <SceneTree.hpp>

#include "../ECSWorld.h"
#include "../Utils/Utils.h"
#include "../Components/AIComponents/FSMStateComponents.h"
#include "../Components/Views/EntityView.h"

void godot::HTHDamagingArea::_register_methods()
{
	register_property<HTHDamagingArea, float>("damage", &HTHDamagingArea::damage, 0);

	register_method((char*)"_on_Area_body_entered", &HTHDamagingArea::_on_Area_body_entered);
	register_method((char*)"_assign_owner_entity", &HTHDamagingArea::_assign_owner_entity);
	register_method((char*)"_ready", &HTHDamagingArea::_ready);
}

void godot::HTHDamagingArea::_ready()
{
	call_deferred("_assign_owner_entity");
}

void godot::HTHDamagingArea::_on_Area_body_entered(KinematicBody* pBody)
{
	ASSERT(pBody != nullptr, "kinematic body is null");
	if (!pBody->has_node("EntityView"))
		return;

	EntityView* pEntityView = Object::cast_to<EntityView>(pBody->get_node("EntityView"));
	entt::entity hittedEntity = pEntityView->GetEntity();

	entt::registry& registry = ECSWorld::GetInstance()->GetRegistry();
	
	float hpBefore = registry.get<HealthComponent>(hittedEntity).hp;
	registry.get<HealthComponent>(hittedEntity).hp -= damage;

	//TODO: remove hardcode
	registry.assign_or_replace<HTHStuckComponent>(hittedEntity, 1.f);
	
	if (registry.has<entt::tag<BotTag> >(hittedEntity))
		registry.assign_or_replace<HittedFromComponent>(hittedEntity, get_global_transform().get_origin());
}

void godot::HTHDamagingArea::_assign_owner_entity()
{
	ASSERT(get_owner()->has_node("../EntityView"), "there is no entity view in hierarchy");
	EntityView* pEntityView = Object::cast_to<EntityView>(get_owner()->get_node("../EntityView"));
	ASSERT(pEntityView != nullptr, "cant cast to EntityView");
	ownerEntity = pEntityView->GetEntity();
	ASSERT(ECSWorld::GetInstance()->GetRegistry().valid(ownerEntity), "HTHDamagingArea invalid holder entity");
}
