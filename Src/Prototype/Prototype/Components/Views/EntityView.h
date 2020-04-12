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

#define DECLARE_CONVERT_TO_COMPONENT(count) template<typename T, int fieldsCount> \
inline typename std::enable_if<fieldsCount == count, T>::type \
ConvertComponentFromArray(godot::Array& arr)

namespace godot
{
	class EntityView : public Node
	{
		GODOT_CLASS(EntityView, Node)
	private:
		entt::entity entity = entt::null;
		Dictionary componentsDict;
		
		template<typename Type, typename... Types>
		void ConstructComponentsFromViews(entt::registry& registry, entt::entity entity);

		DECLARE_CONVERT_TO_COMPONENT(1) { return { arr[0] }; }
		DECLARE_CONVERT_TO_COMPONENT(2) { return { arr[0], arr[1] }; }
		DECLARE_CONVERT_TO_COMPONENT(3) { return { arr[0], arr[1], arr[2] }; }
		DECLARE_CONVERT_TO_COMPONENT(4) { return { arr[0], arr[1], arr[2], arr[3] }; }
		DECLARE_CONVERT_TO_COMPONENT(5) { return { arr[0], arr[1], arr[2], arr[3], arr[4] }; }
	public:
		static void _register_methods() { register_property<EntityView, Dictionary>("components", &EntityView::componentsDict, Dictionary()); }
		void _init() {}

		entt::entity GetEntity() const { return entity; }
		void SetEntity(entt::entity value) { entity = value; }

		template<typename T>
		bool ConvertToComponent(T& comp);

		//TODO: use entity field after merging with EntityHolderNode (merge done)
		void ConstructComponents(entt::registry& registry, entt::entity entity)
		{
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
	inline bool EntityView::ConvertToComponent(T& comp)
	{
		if (!componentsDict.has(ComponentMeta<T>::name))
			return false;

		Array arr = componentsDict[ComponentMeta<T>::name];
		//TODO0: assert right num of values
		comp = ConvertComponentFromArray<T, ComponentMeta<T>::propertiesCount>(arr);
	}

	template<typename Type, typename... Types>
	void EntityView::ConstructComponentsFromViews(entt::registry& registry, entt::entity entity)
	{
		Type component{};
		if (ConvertToComponent(component))
			registry.assign<Type>(entity, component);
		if constexpr (sizeof...(Types))
			ConstructComponentsFromViews<Types...>(registry, entity);
	}
}