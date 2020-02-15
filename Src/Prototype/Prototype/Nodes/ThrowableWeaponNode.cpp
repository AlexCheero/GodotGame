#include "ThrowableWeaponNode.h"

#include <SceneTree.hpp>

#include "../ECSWorld.h"

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

//TODO: probably move this method (and all similar from other nodes) to some kind of reactive system
//		implemented as static functor
void godot::ThrowableWeaponNode::_on_throwable_collide(EntityHolderNode* pEntityHolder)
{
	Godot::print("throwable hit");
	queue_free();

	if (!pEntityHolder)
		return;

	entt::entity entity = pEntityHolder->GetEntity();
	entt::registry& registry = ECSWorld::GetInstance()->GetRegistry();

	//TODO: probably should check (or assert?) registry.has<entt::tag<DeadTag> >(entity)
	ASSERT(entity != entt::null, "target is null");
	//TODO: it fails with access violation exception on work machine when trying to get info about components from registry
	//		try to use call_deferred if the bug reproduced
	ASSERT(registry.has<HealthComponent>(entity), "hitted entity has no HealthComponent");

	HealthComponent& health = registry.get<HealthComponent>(entity);

	health.hp -= GetDamage();
}
