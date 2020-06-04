#pragma once

#include <core/Godot.hpp>
#include <Node.hpp>

#include "entt/entt.hpp"

#include "../Components/AttackComponents.h"
#include "../Components/SimpleComponents.h"
#include "../Components/AIComponents/NavigationComponents.h"
#include "../Components/AIComponents/PatrolComponents.h"
#include "../Components/AIComponents/FSMStateComponents.h"

namespace godot
{
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
		T ConvertComponentFromDict_impl(const Dictionary& dict, std::index_sequence<I...>);

		template<typename T>
		T ConvertComponentFromDict(const Dictionary& dict);

	public:
		static void _register_methods();
		void _init() {}

		entt::entity GetEntity() const { return entity; }
		void SetEntity(entt::entity value) { entity = value; }

		template<typename T>
		bool ConvertToComponent(T& comp);

		//TODO: remove script after initing components
		//TODO: use entity field after merging with EntityHolderNode (merge done) or split it into two separate classes back
		void ConstructComponents(entt::registry& registry, entt::entity entity);
		void ConstructTags(entt::registry& registry, entt::entity entity);
	};
}