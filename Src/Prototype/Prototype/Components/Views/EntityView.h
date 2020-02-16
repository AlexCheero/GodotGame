#pragma once

#include <core/Godot.hpp>
#include <Node.hpp>

#include <map>

#include "entt/entt.hpp"

#include "ComponentView.h"
#include "../AttackComponents.h"
#include "AttackViews/MeleeAttackView.h"
#include "AttackViews/RangedAttackView.h"
#include "AttackViews/ThrowableAttackView.h"
#include "AttackViews/GrenadeView.h"
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
		//TODO: remove all child nodes and use only one node with all components as fields
		//		same as for AnimationTree parameters in inspector
		std::map<String, ComponentView*> componentsMap;

		template<typename Type, typename... Types>
		void ConstructComponentsFromViews(entt::registry& registry, entt::entity entity/*, Types... types*/);
	public:
		static void _register_methods() { register_method((char*)"_ready", &EntityView::_ready); }
		void _init() {}
		void _ready();
		template<typename T>
		bool ConstructComponent(T& component);
		
		void ConstructComponents(entt::registry& registry, entt::entity entity)
		{
			ConstructComponentsFromViews
				<
					MeleeAttackComponent,
					RangedAttackComponent,
					ThrowableAttackComponent,
					HealthComponent,
					GravityComponent,
					JumpSpeedComponent,
					SpeedComponent,
					SimpleFollowComponent,
					PatrolmanComponent,
					NavMarginComponent
				>
				(registry, entity);
		}
	};

	template<typename Type, typename... Types>
	void EntityView::ConstructComponentsFromViews(entt::registry& registry, entt::entity entity)
	{
		Type component{};
		if (ConstructComponent(component))
			registry.assign<Type>(entity, component);
		if constexpr (sizeof...(Types))
			ConstructComponentsFromViews<Types...>(registry, entity);
	}

	template<typename T>
	inline bool EntityView::ConstructComponent(T& component)
	{
		ASSERT(false, "specialized version of this method should be called! Define it with macro CONSTRUCT_COMPONENT!");
		return false;
	}

	//1. names should be equal, differes only with View/Component in the end
	//2. define specialized template method with CONSTRUCT_COMPONENT macro in EntityView.h
	//3. register view in GodotLibrary.cpp
	//4. Component view should implement GetECSComponent method
	//5. add new component type to template arguments in ConstructComponents method
	//6. scene node must have equal view name
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
	CONSTRUCT_COMPONENT(Grenade)
}