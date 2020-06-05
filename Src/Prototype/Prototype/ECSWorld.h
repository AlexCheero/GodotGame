#pragma once

#include <core/Godot.hpp>
#include <Node.hpp>
#include <InputEvent.hpp>

#include "Components/SimpleComponents.h"
#include "Nodes/EntityView.h"

#include "Singleton.h"

namespace godot
{
	class ECSWorld : public Node, public Singleton<ECSWorld>
	{
		GODOT_CLASS(ECSWorld, Node)
	private:
		entt::registry registry;
		using SystemsVec = std::vector<std::function<void(float, entt::registry&)>>;
		SystemsVec physics_systems;
		SystemsVec process_systems;

		template<typename T>
		T* AssignNodeInheritedComponent(entt::registry& registry, entt::entity entity, Node* pNode);

		template<typename Type, typename... Types>
		void PrepareEcsEventsClearingSystems(SystemsVec systems);

		void UpdateSystems(float delta, SystemsVec& systems);

		void PreparePlayerEntity();
		void PrepareEnemyEntity();
		void PrepareSingletonEntities();
		void LoadConfig();

	public:
		entt::registry& GetRegistry() { return registry; }

		static void _register_methods();
		void _init();
		void _ready();
		void HandleInputEvent(InputEvent * e);
		void _process(float delta);
		void _physics_process(float delta);

		std::vector<MeleeHit> LoadHits(String hitsConfigName);
	};
}