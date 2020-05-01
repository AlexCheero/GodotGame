#include "PickableNode.h"

#include <SceneTree.hpp>

#include "../ECSWorld.h"

void godot::PickableNode::SetType(int i)
{
	if (i <= static_cast<int>(EPickableType::None) || i >= static_cast<int>(EPickableType::Last))
		return;

	type = static_cast<EPickableType>(i);
}

void godot::PickableNode::_on_Pickable_body_entered(KinematicBody* pBody)
{
	ASSERT(pBody != nullptr, "kinematic body is null");
	if (!pBody->has_node("EntityView"))
		return;

	queue_free();
	
	EntityView* pPickerEntityView = Object::cast_to<EntityView>(pBody->get_node("EntityView"));
	entt::entity pickerEntity = pPickerEntityView->GetEntity();
	ASSERT(pickerEntity != entt::null, "picker is null");
	EntityView* pPickableView = Object::cast_to<EntityView>(get_node("EntityView"));
	ASSERT(pPickableView != nullptr, "pickable view is null");

	entt::registry& registry = ECSWorld::GetInstance()->GetRegistry();

	//TODO: pick/change weapon by button press
	//		and probably not switch on pickup
	switch (type)
	{
	case EPickableType::MeleeWeapon:
	{
		MeleeWeaponComponent meleeWeaponComp;
		bool constructed = pPickableView->ConvertToComponent(meleeWeaponComp);
		ASSERT(constructed, "can't construct MeleeAttackComponent");
		ASSERT(registry.has<MeleeAttackComponent>(pickerEntity), "entity has no MeleeAttackComponent");
		registry.replace<MeleeAttackComponent>(pickerEntity, ECSWorld::GetInstance()->LoadHits(meleeWeaponComp.hitsConfigName));
		//if switch on pickup
		registry.get_or_assign<CurrentWeaponMeleeTag>(pickerEntity);
		break;
	}
	case EPickableType::RangedWeapon:
	{
		bool constructed = pPickableView->ConvertToComponent(registry.assign_or_replace<RangedAttackComponent>(pickerEntity));
		ASSERT(constructed, "can't construct RangedAttackComponent");
		//if switch on pickup
		registry.get_or_assign<CurrentWeaponRangedTag>(pickerEntity);
		break;
	}
	case EPickableType::ThrowableWeapon:
	{
		bool constructed = pPickableView->ConvertToComponent(registry.assign_or_replace<ThrowableAttackComponent>(pickerEntity));
		ASSERT(constructed, "can't construct ThrowableAttackComponent");
		//if switch on pickup
		registry.get_or_assign<CurrentWeaponThrowableTag>(pickerEntity);
		break;
	}
	case EPickableType::Medkit:
		Godot::print("Picked up Medkit");
		break;
	case EPickableType::Buff:
		Godot::print("Picked up Buff");
		break;
	case EPickableType::Key:
		Godot::print("Picked up Key");
		break;
	default:
		Godot::print_error("Wrong pickable type: " + String::num_int64((int64_t)type), "_on_Pickable_body_entered", "PickableNode.cpp", __LINE__);
		break;
	}
}

void godot::PickableNode::_register_methods()
{
	register_property<PickableNode, int>("type", &PickableNode::SetType, &PickableNode::GetType, -1
		, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM
		, "MeleeWeapon, RangedWeapon, ThrowableWeapon, Medkit, Buff, Key");

	register_method((char*)"_on_Pickable_body_entered", &PickableNode::_on_Pickable_body_entered);
}