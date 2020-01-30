#include "ThrowableWeaponNode.h"

#include <SceneTree.hpp>

#include "../Nodes/EntityHolderNode.h"

void godot::ThrowableWeaponNode::_register_methods()
{
	register_property<ThrowableWeaponNode, float>("damage per force", &ThrowableWeaponNode::damagePerForce, 0);

	register_method((char*)"_on_throwable_collide", &ThrowableWeaponNode::_on_throwable_collide);
	register_method((char*)"_ready", &ThrowableWeaponNode::_ready);
}

void godot::ThrowableWeaponNode::_init()
{
	hittedEntity = entt::null;

	set_contact_monitor(true);
	set_max_contacts_reported(1);
}

void godot::ThrowableWeaponNode::_ready()
{
	Node* world = get_tree()->get_current_scene();
	Array params;
	params.push_back(this);
	connect("body_entered", world, "_on_Throwable_hit", params);
}

void godot::ThrowableWeaponNode::_on_throwable_collide(Node* pNode)
{
	if (hittedEntity != entt::null)
		return;

	EntityHolderNode* pEnemy = Object::cast_to<EntityHolderNode>(pNode);
	if (pEnemy)
		hittedEntity = pEnemy->GetEntity();

	queue_free();
}
