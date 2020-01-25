#include "PickableNodeComponent.h"

void godot::PickableNodeComponent::_register_methods()
{
	register_method((char*)"_on_Pickable_body_entered", &PickableNodeComponent::_on_Pickable_body_entered);
}

void godot::PickableNodeComponent::_on_Pickable_body_entered(Object* body)
{
	Godot::print("entered!");
}