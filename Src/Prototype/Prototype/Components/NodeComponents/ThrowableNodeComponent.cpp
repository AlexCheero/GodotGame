#include "ThrowableNodeComponent.h"

#include "EntityHolderNodeComponent.h"

void godot::ThrowableNodeComponent::_register_methods()
{
	register_property<ThrowableNodeComponent, float>("damage", &ThrowableNodeComponent::damagePerForce, 0);
	register_method((char*)"_on_throwable_collide", &ThrowableNodeComponent::_on_throwable_collide);
}

void godot::ThrowableNodeComponent::_init()
{
	hittedEntity = entt::null;

	set_contact_monitor(true);
	set_max_contacts_reported(1);
}

void godot::ThrowableNodeComponent::_on_throwable_collide(Node* pNode)
{
	call_deferred("set_contact_monitor", false);

	EntityHolderNodeComponent* pEnemy = Object::cast_to<EntityHolderNodeComponent>(pNode);
	if (pEnemy)
		hittedEntity = pEnemy->GetEntity();
}
