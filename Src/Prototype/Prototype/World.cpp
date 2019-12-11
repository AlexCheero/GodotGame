#include "World.h"

#include <SceneTree.hpp>
#include <KinematicBody.hpp>

#include "Systems/PlayerVelocitySystem.h"
#include "Systems/KinematicMovementSystem.h"
#include "Systems/GravitySystem.h";

void godot::World::CleanUpSystems(std::vector<BaseSystem*>& systems)
{
	for (BaseSystem* system : systems)
	{
		if (!system)
			continue;
		delete system;
		system = nullptr;
	}
}

void godot::World::UpdateSystems(float delta, std::vector<BaseSystem*>& systems)
{
	for (BaseSystem* system : systems)
		(*system)(delta, registry);
}

godot::World::~World()
{
	CleanUpSystems(m_process_systems);
	CleanUpSystems(m_physics_systems);
}

void godot::World::_register_methods()
{
	register_method((char*)"_init", &World::_init);
	register_method((char*)"_ready", &World::_ready);
	register_method((char*)"_input", &World::HandleInputEvent);
	register_method((char*)"_process", &World::_process);
	register_method((char*)"_physics_process", &World::_physics_process);
}

void godot::World::_init()
{
	//set up systems
	m_physics_systems.insert(m_physics_systems.end(), new PlayerVelocitySystem());
	m_physics_systems.insert(m_physics_systems.end(), new KinematicMovementSystem());
	m_physics_systems.insert(m_physics_systems.end(), new GravitySystem());
}

void godot::World::_ready()
{
	//create entities and components
	//<Player entity
	entt::entity entity = registry.create();
	registry.assign<VelocityComponent>(entity);
	registry.assign<SpeedComponent>(entity, 30.f);
	KinematicBody* pBody = Object::cast_to<KinematicBody>(get_node("Player"));
	registry.assign<KinematicBody*>(entity, pBody);
	registry.assign<GravityComponent>(entity, GravityComponent{ 30, 20 });
	//Player entity>
}

void godot::World::HandleInputEvent(InputEvent* e)
{
	if (e->is_action_pressed("ui_accept"))
	{
		//<todo reload registry
		registry.reset();
		_ready();
		//todo reload registry>
		get_tree()->reload_current_scene();
	}
	else if (e->is_action_pressed("ui_cancel"))
		get_tree()->quit();
}

void godot::World::_process(float delta)
{
	UpdateSystems(delta, m_process_systems);
}

void godot::World::_physics_process(float delta)
{
	UpdateSystems(delta, m_physics_systems);
}
