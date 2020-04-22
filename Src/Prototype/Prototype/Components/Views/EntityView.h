#pragma once

#include <core/Godot.hpp>
#include <Node.hpp>

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

namespace godot
{
	class EntityView : public Node
	{
		GODOT_CLASS(EntityView, Node)
	private:
		entt::entity entity = entt::null;
		Dictionary componentsDict;

#ifdef DEBUG
		template<typename T>
		bool CheckKeys(Dictionary& dict)
		{
			if (dict.size() != ComponentMeta<T>::propertiesCount)
				return false;

			for (int i = 0; i < ComponentMeta<T>::propertiesCount; i++)
			{
				if (!dict.has(ComponentMeta<T>::properties[i]))
					return false;
			}

			return true;
		}
#endif
		
		template<typename Type, typename... Types>
		void ConstructComponentsFromViews(entt::registry& registry, entt::entity entity);

		template<typename T, std::size_t... I>
		T ConvertComponentFromDict_impl(const Dictionary& dict, std::index_sequence<I...>)
		{
			return T{ dict[ComponentMeta<T>::properties[I]]... };
		}

		template<typename T>
		T ConvertComponentFromDict(const Dictionary& dict)
		{
			return ConvertComponentFromDict_impl<T>(dict, std::make_index_sequence<ComponentMeta<T>::propertiesCount>{});
		}

	public:
		static void _register_methods() { register_property<EntityView, Dictionary>("components", &EntityView::componentsDict, Dictionary()); }
		void _init() {}

		entt::entity GetEntity() const { return entity; }
		void SetEntity(entt::entity value) { entity = value; }

		template<typename T>
		bool ConvertToComponent(T& comp);

		//TODO: remove script after initing components
		//TODO: use entity field after merging with EntityHolderNode (merge done)
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

	template<typename T>
	inline bool EntityView::ConvertToComponent(T& comp)
	{
		if (!componentsDict.has(ComponentMeta<T>::name))
			return false;

		Dictionary dict = componentsDict[ComponentMeta<T>::name];

#ifdef DEBUG
		ASSERT(CheckKeys<T>(dict), "wrong keys in components dict of entity view");
#endif

		comp = ConvertComponentFromDict<T>(dict);
		
		return true;
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