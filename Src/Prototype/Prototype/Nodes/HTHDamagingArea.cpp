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
	register_method((char*)"_ready", &HTHDamagingArea::_ready);
}

void godot::HTHDamagingArea::_ready()
{
	call_deferred("_assign_owner_entity");
}

void godot::HTHDamagingArea::_on_Area_body_entered(EntityHolderNode* pEntityHolder)
{
	if (!pEntityHolder)
		return;

	entt::registry& registry = ECSWorld::GetInstance()->GetRegistry();

	entt::entity hittedEntity = pEntityHolder->GetEntity();
	
	float hpBefore = registry.get<HealthComponent>(hittedEntity).hp;
	registry.get<HealthComponent>(hittedEntity).hp -= damage;

	String parentName = get_parent()->get_name();
	Godot::print(parentName + " collided. hp before: " + String::num(hpBefore, 0) + ", hp after: " +
		String::num(registry.get<HealthComponent>(hittedEntity).hp, 0) + ", damage: " + String::num(damage, 0));
	
	if (!registry.has<HittedByComponent>(hittedEntity))
		registry.assign<HittedByComponent>(hittedEntity).attacker = ownerEntity;
	
	//TODO: implement instead of HittedByComponent
	//registry.assign<HittedFromComponent>(hittedEntity).position = get_global_transform().get_origin();
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
