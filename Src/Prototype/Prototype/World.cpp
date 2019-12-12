#include "World.h"

#include <SceneTree.hpp>
#include <KinematicBody.hpp>
#include <Node.hpp>
#include <Camera.hpp>

#include "Components/Player.h"

#include "Systems/PlayerVelocitySystem.h"
#include "Systems/KinematicMovementSystem.h"
#include "Systems/GravitySystem.h";
#include "Systems/JumpSystem.h";
#include "Systems/PlayerRotationSystem.h"
#include "Systems/CameraFollowSystem.h"

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
	//setup physics systems
	m_physics_systems.insert(m_physics_systems.end(), new PlayerVelocitySystem());
	m_physics_systems.insert(m_physics_systems.end(), new KinematicMovementSystem());
	m_physics_systems.insert(m_physics_systems.end(), new GravitySystem());
	//TODO: must always follow GravitySystem. find a way to enforce such behaviour in entt
	m_physics_systems.insert(m_physics_systems.end(), new JumpSystem());
	m_physics_systems.insert(m_physics_systems.end(), new PlayerRotationSystem());

	//setup systems
	m_process_systems.insert(m_process_systems.end(), new CameraFollowSystem());
}

void godot::World::_ready()
{
	//create entities and components
	//<Player entity
	entt::entity playerEntity = registry.create();
	
	Node* pPlayerNode = get_node("Player");
	
	Player* pPlayer = Object::cast_to<Player>(pPlayerNode);
	registry.assign<Player*>(playerEntity, pPlayer);
	
	KinematicBody* pBody = Object::cast_to<KinematicBody>(pPlayerNode);
	registry.assign<KinematicBody*>(playerEntity, pBody);
	
	registry.assign<GravityComponent>(playerEntity, GravityComponent{ 30, 20 });
	registry.assign<JumpSpeedComponent>(playerEntity, JumpSpeedComponent{ 30 });//find a way to set these values via editor
	registry.assign<RotationComponent>(playerEntity);
	registry.assign<VelocityComponent>(playerEntity);
	registry.assign<SpeedComponent>(playerEntity, 30.f);
	//Player entity>

	//<Camera entity
	entt::entity cameraEntity = registry.create();
	
	Camera* pCamera = Object::cast_to<Camera>(get_node("Camera"));
	registry.assign<Camera*>(cameraEntity, pCamera);
	registry.assign<Camera*>(playerEntity, pCamera);
	
	registry.assign<CamPositionComponent>(cameraEntity, CamPositionComponent{ 15, 30, 45 });
	registry.assign<Spatial*>(cameraEntity, Object::cast_to<Spatial>(pPlayerNode));
	//Camera entity>
}

void godot::World::HandleInputEvent(InputEvent* e)
{
	if (e->is_action_pressed("ui_accept"))
	{
		registry.reset();
		_ready();
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
