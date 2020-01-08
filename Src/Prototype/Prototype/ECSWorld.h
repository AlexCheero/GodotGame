#pragma once

#include <core/Godot.hpp>
#include <Node.hpp>
#include <InputEvent.hpp>

#include "Systems/BaseSystem.h"

#include "Components/SimpleComponents.h"

namespace godot
{
	class ECSWorld : public Node
	{
		GODOT_CLASS(ECSWorld, Node)
	private:
		entt::registry registry;
		using SystemsVec = std::vector<std::unique_ptr<BaseSystem>>;
		SystemsVec m_physics_systems;
		SystemsVec m_process_systems;

		template<typename T>
		T* AssignNodeInheritedComponent(entt::registry& registry, entt::entity entity, Node* pNode)
		{
			T* pComp = Object::cast_to<T>(pNode);
			return registry.assign<T*>(entity, pComp);
		}

		void UpdateSystems(float delta, SystemsVec& systems);

		void PreparePlayerEntity();
		void PrepareCameraEntity();
		void PrepareEnemyEntity();
		BoundsComponent GetCapsuleBounds(Node* pCapsuleNode);
	public:
		static void _register_methods();
		void _init();
		void _ready();
		void HandleInputEvent(InputEvent * e);
		void _process(float delta);
		void _physics_process(float delta);
	};
}