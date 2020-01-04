#include "ThrowableNodeComponent.h"

#include "EnemyNodeComponent.h"

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

	EnemyNodeComponent* pEnemy = Object::cast_to<EnemyNodeComponent>(pNode);
	if (pEnemy)
		hittedEntity = pEnemy->GetEntity();
}
