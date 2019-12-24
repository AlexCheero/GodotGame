#include "Throwable.h"

#include "Enemy.h"

void godot::Throwable::_register_methods()
{
	register_method((char*)"_on_throwable_collide", &Throwable::_on_throwable_collide);
}

void godot::Throwable::_init()
{
	hittedEntity = entt::null;
	set_contact_monitor(true);
	set_max_contacts_reported(1);
}

void godot::Throwable::_on_throwable_collide(Node* pNode)
{
	call_deferred("set_contact_monitor", false);

	Enemy* pEnemy = Object::cast_to<Enemy>(pNode);
	if (pEnemy)
		hittedEntity = pEnemy->GetEntity();
}
