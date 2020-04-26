#include "ThrowableWeaponNode.h"

#include <SceneTree.hpp>

#include "../ECSWorld.h"
#include "../Nodes/EntityView.h"

void godot::ThrowableWeaponNode::_register_methods()
{
	register_property<ThrowableWeaponNode, float>("damage per force", &ThrowableWeaponNode::damagePerForce, 0);

	register_method((char*)"_on_throwable_collide", &ThrowableWeaponNode::_on_throwable_collide);
}

void godot::ThrowableWeaponNode::_init()
{
	set_contact_monitor(true);
	set_max_contacts_reported(1);
}

void godot::ThrowableWeaponNode::_on_throwable_collide(KinematicBody* pBody)
{
	Godot::print("throwable hit");
	queue_free();

	ASSERT(pBody != nullptr, "kinematic body is null");
	if (!pBody->has_node("EntityView"))
		return;

	EntityView* pEntityView = Object::cast_to<EntityView>(pBody->get_node("EntityView"));
	entt::entity entity = pEntityView->GetEntity();
	entt::registry& registry = ECSWorld::GetInstance()->GetRegistry();

	ASSERT(registry.valid(entity), "target is null");
	ASSERT(!registry.has<DeadTag>(entity), "entity is already dead");
	ASSERT(registry.has<HealthComponent>(entity), "hitted entity has no HealthComponent");

	HealthComponent& health = registry.get<HealthComponent>(entity);

	health.hp -= GetDamage();
}
