//TODO: probably have to change all #pragma once to include guards, to compile with other compiler
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
		using SystemsVec = std::vector<std::unique_ptr<BaseSystem>>;
		SystemsVec m_physics_systems;
		SystemsVec m_process_systems;

		void UpdateSystems(float delta, SystemsVec& systems);

		void PreparePlayerEntity();
		void PrepareCameraEntity();
		void PrepareEnemyEntity();
	public:
		static void _register_methods();
		void _init();
		void _ready();
		void HandleInputEvent(InputEvent * e);
		void _process(float delta);
		void _physics_process(float delta);
	};
}