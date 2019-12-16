#pragma once

#include <core/Godot.hpp>
#include <Node.hpp>
#include <InputEvent.hpp>

#include "Systems\BaseSystem.h"

namespace godot
{
	class ECSWorld : public Node
	{
		GODOT_CLASS(ECSWorld, Node)
	private:
		entt::registry registry;
		std::vector<BaseSystem*> m_physics_systems;
		std::vector<BaseSystem*> m_process_systems;

		void CleanUpSystems(std::vector<BaseSystem*>& systems);
		void UpdateSystems(float delta, std::vector<BaseSystem*>& systems);

		void PreparePlayerEntity();
		void PrepareCameraEntity();
		void PrepareEnemyEntity();
	public:
		ECSWorld() = default;
		virtual ~ECSWorld();

		static void _register_methods();
		void _init();
		void _ready();
		void HandleInputEvent(InputEvent * e);
		void _process(float delta);
		void _physics_process(float delta);
	};
}