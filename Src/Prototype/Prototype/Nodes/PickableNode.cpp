#include "PickableNode.h"

#include <SceneTree.hpp>

#include "../Nodes/EntityHolderNode.h"

void godot::PickableNode::SetType(int i)
{
	if (i <= static_cast<int>(EPickableType::None) || i >= static_cast<int>(EPickableType::Last))
		return;

	type = static_cast<EPickableType>(i);
}

void godot::PickableNode::_on_Pickable_body_entered(Node* pNode)
{
	EntityHolderNode* pEntityHolder = Object::cast_to<EntityHolderNode>(pNode);
	if (pickerEntity != entt::null || !pEntityHolder)
		return;

	pickerEntity = pEntityHolder->GetEntity();
	queue_free();
}

void godot::PickableNode::_register_methods()
{
	register_property<PickableNode, int>("type", &PickableNode::SetType, &PickableNode::GetType, -1
		, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM
		, "MeleeWeapon, RangedWeapon, ThrowableWeapon, Medkit, Buff, Key");

	register_method((char*)"_ready", &PickableNode::_ready);
	register_method((char*)"_on_Pickable_body_entered", &PickableNode::_on_Pickable_body_entered);
}

void godot::PickableNode::_init()
{
	pickerEntity = entt::null;
}

void godot::PickableNode::_ready()
{
	Node* world = get_tree()->get_current_scene();
	Array params;
	params.push_back(get_node("EntityView"));
	params.push_back(static_cast<int>(type));
	connect("body_entered", world, "_on_Pickable_picked_up", params);
}