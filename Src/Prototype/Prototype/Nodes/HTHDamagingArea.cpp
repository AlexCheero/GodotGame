#include "HTHDamagingArea.h"

#include <SceneTree.hpp>

#include "../ECSWorld.h"
#include "../Utils/Utils.h"
#include "EntityHolderNode.h"
#include "../Components/AIComponents/FSMStateComponents.h"

void godot::HTHDamagingArea::_register_methods()
{
	register_property<HTHDamagingArea, float>("damage", &HTHDamagingArea::damage, 0);

	register_method((char*)"_on_Area_body_entered", &HTHDamagingArea::_on_Area_body_entered);
	register_method((char*)"_assign_owner_entity", &HTHDamagingArea::_assign_owner_entity);
}

void godot::HTHDamagingArea::_init()
{
	call_deferred("_assign_owner_entity");
}

void godot::HTHDamagingArea::_on_Area_body_entered(EntityHolderNode* pEntityHolder)
{
	if (!pEntityHolder)
		return;

	entt::registry& registry = ECSWorld::GetInstance()->GetRegistry();

	entt::entity hittedEntity = pEntityHolder->GetEntity();
	registry.get<HealthComponent>(hittedEntity).hp -= damage;
	//TODO0: make full transition to pursuing fsm state
	registry.assign_or_replace<PursuingStateComponent>(hittedEntity).target = ownerEntity;
}

void godot::HTHDamagingArea::_assign_owner_entity()
{
	EntityHolderNode* holderNode = utils::GetParentOfType<EntityHolderNode>(this);
	if (holderNode == nullptr)
		return;

	ASSERT(holderNode != nullptr, "HTHDamagingArea have no holder");
	ownerEntity = holderNode->GetEntity();
	ASSERT(ECSWorld::GetInstance()->GetRegistry().valid(ownerEntity), "HTHDamagingArea invalid holder entity");
}
