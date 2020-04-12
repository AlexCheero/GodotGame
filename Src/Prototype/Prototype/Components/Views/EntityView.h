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

//TODO0: move here and remove converter file
#include "ComponentConverter.h"

//#define CONSTRUCT_COMPONENT(Name) template<> \
//inline bool EntityView::ConstructComponent<Name ## Component>(Name ## Component& component) \
//{ \
//	auto it = componentsMap.find(#Name); \
//	if (it != componentsMap.end()) \
//	{ \
//		Name ## View* view = Object::cast_to<Name ## View>(it->second); \
//		component = view->GetECSComponent(); \
//		return true; \
//	} \
//	else \
//		return false; \
//}

#define COMPONENTS_NAME(type) #type

namespace godot
{
	class EntityView : public Node
	{
		GODOT_CLASS(EntityView, Node)
	private:
		entt::entity entity = entt::null;
		//TODO: remove all child nodes and use only one node with all components as fields
		//		same as for AnimationTree parameters in inspector
		//		probably merge with entity holder node
		//std::map<String, ComponentView*> componentsMap;

		Dictionary componentsDict;
		
		template<typename Type, typename... Types>
		void ConstructComponentsFromViews(entt::registry& registry, entt::entity entity);
	public:
		static void _register_methods();
		void _init() {}
		//void _ready();

		entt::entity GetEntity() const { return entity; }
		void SetEntity(entt::entity value) { entity = value; }

		template<typename T>
		bool ConvertToComponent1(T& comp);

		//template<typename T>
		//bool ConstructComponent(T& component);
		
		//TODO: use entity field after merging with EntityHolderNode (merge done)
		void ConstructComponents(entt::registry& registry, entt::entity entity)
		{
			Godot::print("construct components for " + get_parent()->get_name() + ":");

			//TODO: assert views have no wrong keys
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

	template<typename T>
	inline bool EntityView::ConvertToComponent1(T& comp)
	{
		if (!componentsDict.has(ComponentMeta<T>::name))
			return false;

		Godot::print(String("    convert ") + ComponentMeta<T>::name);

		Array arr = componentsDict[ComponentMeta<T>::name];
		//TODO0: assert right num of values
		comp = ConvertToComponent<T, ComponentMeta<T>::propertiesCount>(arr);
	}

	template<typename Type, typename... Types>
	void EntityView::ConstructComponentsFromViews(entt::registry& registry, entt::entity entity)
	{
		//if (componentsDict.has(COMPONENTS_NAME(Type)))
		//	registry.assign<Type>(entity, ConvertToComponent<Type>());
		//if constexpr (sizeof...(Types))
		//	ConstructComponentsFromViews<Types...>(registry, entity);

		Type component{};
		if (ConvertToComponent1(component))
			registry.assign<Type>(entity, component);
		if constexpr (sizeof...(Types))
			ConstructComponentsFromViews<Types...>(registry, entity);
	}

	//template<typename T>
	//inline bool EntityView::ConstructComponent(T& component)
	//{
	//	ASSERT(false, "specialized version of this method should be called! Define it with macro CONSTRUCT_COMPONENT!");
	//	return false;
	//}

	//1. names should be equal, differes only with View/Component in the end
	//2. define specialized template method with CONSTRUCT_COMPONENT macro in EntityView.h
	//3. register view in GodotLibrary.cpp
	//4. Component view should implement GetECSComponent method
	//5. add new component type to template arguments in ConstructComponents method
	//6. scene node must have equal view name
	//CONSTRUCT_COMPONENT(MeleeAttack)
	//CONSTRUCT_COMPONENT(RangedAttack)
	//CONSTRUCT_COMPONENT(ThrowableAttack)
	//CONSTRUCT_COMPONENT(Health)
	//CONSTRUCT_COMPONENT(Gravity)
	//CONSTRUCT_COMPONENT(JumpSpeed)
	//CONSTRUCT_COMPONENT(Speed)
	//CONSTRUCT_COMPONENT(SimpleFollow)
	//CONSTRUCT_COMPONENT(Patrolman)
	//CONSTRUCT_COMPONENT(NavMargin)
	//CONSTRUCT_COMPONENT(Grenade)
}