#pragma once

#include <core/Godot.hpp>
#include <Node.hpp>

#include "entt/entt.hpp"

#include "../Components/AttackComponents.h"
#include "../Components/SimpleComponents.h"
#include "../Components/AIComponents/NavigationComponents.h"
#include "../Components/AIComponents/PatrolComponents.h"

namespace godot
{
	//TODO: implement tag views
	class EntityView : public Node
	{
		GODOT_CLASS(EntityView, Node)
	private:
		entt::entity entity = entt::null;
		Dictionary componentsDict;
		Array tags;

#ifdef DEBUG
		template<typename T>
		bool CheckKeys(Dictionary& dict);
#endif
		
		template<typename Type, typename... Types>
		void ConstructComponents_impl(entt::registry& registry, entt::entity entity);

		template<typename Tag, typename... Tags>
		void ConstructTags_impl(entt::registry& registry, entt::entity entity);

		template<typename Tag>
		bool HasTag();

		template<typename T, std::size_t... I>
		T ConvertComponentFromDict_impl(const Dictionary& dict, std::index_sequence<I...>)
		{
			return T{ dict[ComponentsMeta<T>::properties[I]]... };
		}

		template<typename T>
		T ConvertComponentFromDict(const Dictionary& dict)
		{
			return ConvertComponentFromDict_impl<T>(dict, std::make_index_sequence<ComponentsMeta<T>::propertiesCount>{});
		}

	public:
		static void _register_methods()
		{
			register_property<EntityView, Dictionary>("components", &EntityView::componentsDict, Dictionary());
			register_property<EntityView, Array>("tags", &EntityView::tags, Array());
		}
		void _init() {}

		entt::entity GetEntity() const { return entity; }
		void SetEntity(entt::entity value) { entity = value; }

		template<typename T>
		bool ConvertToComponent(T& comp);

		//TODO: remove script after initing components
		//TODO: use entity field after merging with EntityHolderNode (merge done) or split it into two separate classes back
		void ConstructComponents(entt::registry& registry, entt::entity entity)
		{
			ConstructComponents_impl
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
					NavMarginComponent,
					MeleeWeaponComponent
				>
				(registry, entity);
		}

		void ConstructTags(entt::registry& registry, entt::entity entity)
		{
			ConstructTags_impl
				<
					PlayerTag,
					BotTag,
					CurrentWeaponMeleeTag,
					CurrentWeaponRangedTag,
					CurrentWeaponThrowableTag,
					MainCameraTag
				>
				(registry, entity);
		}
	};

	template<typename T>
	inline bool EntityView::ConvertToComponent(T& comp)
	{
		String componentKey(ComponentsMeta<T>::typeName);
		//assert that component's type name always ends with "Component"
		ASSERT(componentKey.find_last("Component") == componentKey.length() - 9, "component's type name doesn't ends with \"Component\"");
		componentKey = componentKey.substr(0, componentKey.length() - 9);
		if (!componentsDict.has(componentKey))
			return false;

		Dictionary dict = componentsDict[componentKey];

#ifdef DEBUG
		ASSERT(CheckKeys<T>(dict), "wrong keys in components dict of entity view");
#endif

		comp = ConvertComponentFromDict<T>(dict);
		
		return true;
	}

#ifdef DEBUG
	template<typename T>
	inline bool EntityView::CheckKeys(Dictionary& dict)
	{
		if (dict.size() != ComponentsMeta<T>::propertiesCount)
			return false;

		for (int i = 0; i < ComponentsMeta<T>::propertiesCount; i++)
		{
			if (!dict.has(ComponentsMeta<T>::properties[i]))
				return false;
		}

		return true;
	}
#endif

	template<typename Type, typename... Types>
	void EntityView::ConstructComponents_impl(entt::registry& registry, entt::entity entity)
	{
		Type component{};
		if (ConvertToComponent(component))
			registry.assign<Type>(entity, component);
		if constexpr (sizeof...(Types))
			ConstructComponents_impl<Types...>(registry, entity);
	}

	template<typename Tag>
	inline bool EntityView::HasTag()
	{
		for (int i = 0; i < tags.size(); i++)
		{
#ifdef DEBUG
			//assert that tag's name always ends with "Tag"
			String debugTagName(TagMeta<Tag>::tagName);
			ASSERT(debugTagName.find_last("Tag") == debugTagName.length() - 3, "tag's name doesn't ends with \"Tag\"");
#endif
			String tagName = String(tags[i]) + "Tag";
			if (tagName == TagMeta<Tag>::tagName)
				return true;
		}

		return false;
	}

	template<typename Tag, typename ...Tags>
	inline void EntityView::ConstructTags_impl(entt::registry& registry, entt::entity entity)
	{
		if (HasTag<Tag>())
			registry.assign<Tag>(entity);
		if constexpr (sizeof...(Tags))
			ConstructTags_impl<Tags...>(registry, entity);
	}
}