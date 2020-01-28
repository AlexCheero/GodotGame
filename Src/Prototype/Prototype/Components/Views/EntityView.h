#pragma once

#include <core/Godot.hpp>
#include <Node.hpp>

#include <map>

#include "ComponentView.h"
#include "../AttackComponents.h"
#include "AttackViews/MeleeAttackView.h"
#include "AttackViews/RangedAttackView.h"
#include "AttackViews/ThrowableAttackView.h"
#include "../SimpleComponents.h"
#include "HealthView.h"
#include "LocomotionViews/GravityView.h"
#include "LocomotionViews/JumpSpeedView.h"
#include "LocomotionViews/SpeedView.h"
#include "SimpleFollowView.h"
#include "AIViews/PatrolmanView.h"
#include "AIViews/NavMarginView.h"

#define CONSTRUCT_COMPONENT(Name) template<> \
inline bool EntityView::ConstructComponent<Name ## Component>(Name ## Component& component) \
{ \
	auto it = componentsMap.find(#Name); \
	if (it != componentsMap.end()) \
	{ \
		Name ## View* view = Object::cast_to<Name ## View>(it->second); \
		component = view->GetECSComponent(); \
		return true; \
	} \
	else \
		return false; \
}

namespace godot
{
	class EntityView : public Node
	{
		GODOT_CLASS(EntityView, Node)
	private:
		std::map<String, ComponentView*> componentsMap;
	public:
		static void _register_methods() { register_method((char*)"_ready", &EntityView::_ready); }
		void _init() {}
		void _ready();
		template<typename T>
		bool ConstructComponent(T& component);
	};

	template<typename T>
	inline bool EntityView::ConstructComponent(T& component)
	{
		Godot::print_error("Define specialized version of this method with macro CONSTRUCT_COMPONENT!");
		return false;
	}

	//1. names should be equal, differes only with View/Component in the end
	//2. define specialized template method with CONSTRUCT_COMPONENT macro in EntityView.h
	//3. register view in GodotLibrary.cpp
	//4. Component view should implement GetECSComponent method
	CONSTRUCT_COMPONENT(MeleeAttack)
	CONSTRUCT_COMPONENT(RangedAttack)
	CONSTRUCT_COMPONENT(ThrowableAttack)
	CONSTRUCT_COMPONENT(Health)
	CONSTRUCT_COMPONENT(Gravity)
	CONSTRUCT_COMPONENT(JumpSpeed)
	CONSTRUCT_COMPONENT(Speed)
	CONSTRUCT_COMPONENT(SimpleFollow)
	CONSTRUCT_COMPONENT(Patrolman)
	CONSTRUCT_COMPONENT(NavMargin)
}