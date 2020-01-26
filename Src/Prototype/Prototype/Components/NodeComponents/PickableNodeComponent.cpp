#include "PickableNodeComponent.h"

#include <SceneTree.hpp>

#include "EntityHolderNodeComponent.h"

void godot::PickableNodeComponent::SetType(int i)
{
	if (i <= static_cast<int>(EPickableType::None) || i >= static_cast<int>(EPickableType::Last))
		return;

	type = static_cast<EPickableType>(i);
}

void godot::PickableNodeComponent::_on_Pickable_body_entered(Node* pNode)
{
	EntityHolderNodeComponent* pEntityHolder = Object::cast_to<EntityHolderNodeComponent>(pNode);
	if (pickerEntity != entt::null || !pEntityHolder)
		return;

	pickerEntity = pEntityHolder->GetEntity();
	queue_free();
}

void godot::PickableNodeComponent::_register_methods()
{
	register_property<PickableNodeComponent, int>("type", &PickableNodeComponent::SetType, &PickableNodeComponent::GetType, -1
		, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM
		, "MeleeWeapon, RangedWeapon, ThrowableWeapon, Medkit, Buff, Key");

	register_method((char*)"_ready", &PickableNodeComponent::_ready);
	register_method((char*)"_on_Pickable_body_entered", &PickableNodeComponent::_on_Pickable_body_entered);
}

void godot::PickableNodeComponent::_init()
{
	pickerEntity = entt::null;
}

void godot::PickableNodeComponent::_ready()
{
	Node* world = get_tree()->get_current_scene();
	Array params;
	params.push_back(get_node("EntityView"));
	params.push_back(static_cast<int>(type));
	connect("body_entered", world, "_on_Pickable_picked_up", params);
}