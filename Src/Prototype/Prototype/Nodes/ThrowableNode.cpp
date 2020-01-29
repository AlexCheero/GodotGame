#include "ThrowableNode.h"

#include <SceneTree.hpp>

#include "../Nodes/EntityHolderNode.h"

void godot::ThrowableNode::_register_methods()
{
	register_property<ThrowableNode, float>("damage", &ThrowableNode::damagePerForce, 0);

	register_method((char*)"_on_throwable_collide", &ThrowableNode::_on_throwable_collide);
	register_method((char*)"_ready", &ThrowableNode::_ready);
}

void godot::ThrowableNode::_init()
{
	hittedEntity = entt::null;

	set_contact_monitor(true);
	set_max_contacts_reported(1);
}

void godot::ThrowableNode::_ready()
{
	Node* world = get_tree()->get_current_scene();
	Array params;
	params.push_back(this);
	connect("body_entered", world, "_on_Throwable_hit", params);
}

void godot::ThrowableNode::_on_throwable_collide(Node* pNode)
{
	if (hittedEntity != entt::null)
		return;

	EntityHolderNode* pEnemy = Object::cast_to<EntityHolderNode>(pNode);
	if (pEnemy)
		hittedEntity = pEnemy->GetEntity();

	queue_free();
}
