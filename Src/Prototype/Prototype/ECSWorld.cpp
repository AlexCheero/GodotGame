#include "ECSWorld.h"

#include <SceneTree.hpp>
#include <KinematicBody.hpp>
#include <Node.hpp>
#include <Camera.hpp>

#include "Components/Player.h"
#include "Components/Enemy.h"

#include "Systems/PlayerVelocitySystem.h"
#include "Systems/KinematicMovementSystem.h"
#include "Systems/GravitySystem.h";
#include "Systems/JumpSystem.h";
#include "Systems/PlayerRotationSystem.h"
#include "Systems/CameraFollowSystem.h"
#include "Systems/AttackSystem.h"
#include "Systems/DestroyDeadSystem.h"

#include "Utils.h"

void godot::ECSWorld::CleanUpSystems(std::vector<BaseSystem*>& systems)
{
	for (BaseSystem* system : systems)
	{
		if (!system)
			continue;
		delete system;
		system = nullptr;
	}
}

void godot::ECSWorld::UpdateSystems(float delta, std::vector<BaseSystem*>& systems)
{
	for (BaseSystem* system : systems)
		(*system)(delta, registry);
}

void godot::ECSWorld::PreparePlayerEntity()
{
	entt::entity entity = registry.create();

	Node* pPlayerNode = get_node("Player");
	registry.assign<Node*>(entity, pPlayerNode);

	Player* pPlayer = Object::cast_to<Player>(pPlayerNode);
	registry.assign<Player*>(entity, pPlayer);

	KinematicBody* pBody = Object::cast_to<KinematicBody>(pPlayerNode);
	registry.assign<KinematicBody*>(entity, pBody);

	Spatial* pSpatial = Object::cast_to<Spatial>(pPlayerNode);
	registry.assign<Spatial*>(entity, pSpatial);

	Camera* pCamera = Object::cast_to<Camera>(get_node("Camera"));
	registry.assign<Camera*>(entity, pCamera);

	registry.assign<GravityComponent>(entity, GravityComponent{ 30, 20 });
	registry.assign<JumpSpeedComponent>(entity, JumpSpeedComponent{ 30 });//find a way to set these values via editor
	registry.assign<RotationComponent>(entity);
	registry.assign<VelocityComponent>(entity);
	registry.assign<SpeedComponent>(entity, 30.f);
	registry.assign<HealthComponent>(entity, 100.f);
	registry.assign<AttackComponent>(entity, 4.f, 50.f, 90.f);
}

void godot::ECSWorld::PrepareCameraEntity()
{
	entt::entity entity = registry.create();

	Camera* pCamera = Object::cast_to<Camera>(get_node("Camera"));
	registry.assign<Camera*>(entity, pCamera);
	registry.assign<CamPositionComponent>(entity, CamPositionComponent{ 15, -30, -45 });
	registry.assign<Spatial*>(entity, Object::cast_to<Spatial>(get_node("Player")));
}

void godot::ECSWorld::PrepareEnemyEntity()
{
	entt::entity entity = registry.create();

	Node* pEnemyNode = get_node("Enemy");
	registry.assign<Node*>(entity, pEnemyNode);

	Enemy* pEnemy = Object::cast_to<Enemy>(pEnemyNode);
	registry.assign<Enemy*>(entity, pEnemy);
	pEnemy->SetEntity(entity);

	Spatial* pSpatial = Object::cast_to<Spatial>(pEnemyNode);
	registry.assign<Spatial*>(entity, pSpatial);

	registry.assign<HealthComponent>(entity, 100.f);
}

godot::ECSWorld::~ECSWorld()
{
	CleanUpSystems(m_process_systems);
	CleanUpSystems(m_physics_systems);
}

void godot::ECSWorld::_register_methods()
{
	register_method((char*)"_init", &ECSWorld::_init);
	register_method((char*)"_ready", &ECSWorld::_ready);
	register_method((char*)"_input", &ECSWorld::HandleInputEvent);
	register_method((char*)"_process", &ECSWorld::_process);
	register_method((char*)"_physics_process", &ECSWorld::_physics_process);
}

void godot::ECSWorld::_init()
{
	utils::InitPhysicLayers();

	//setup physics systems
	m_physics_systems.insert(m_physics_systems.end(), new PlayerVelocitySystem());
	m_physics_systems.insert(m_physics_systems.end(), new KinematicMovementSystem());
	m_physics_systems.insert(m_physics_systems.end(), new GravitySystem());
	//TODO: must always follow GravitySystem. find a way to enforce such behaviour in entt
	m_physics_systems.insert(m_physics_systems.end(), new JumpSystem());
	m_physics_systems.insert(m_physics_systems.end(), new PlayerRotationSystem());
	m_physics_systems.insert(m_physics_systems.end(), new AttackSystem());

	//setup systems
	m_process_systems.insert(m_process_systems.end(), new CameraFollowSystem());
	m_process_systems.insert(m_process_systems.end(), new DestroyDeadSystem());
}

void godot::ECSWorld::_ready()
{
	//create entities and components
	PreparePlayerEntity();
	PrepareCameraEntity();
	PrepareEnemyEntity();
}

void godot::ECSWorld::HandleInputEvent(InputEvent* e)
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

void godot::ECSWorld::_process(float delta)
{
	UpdateSystems(delta, m_process_systems);
}

void godot::ECSWorld::_physics_process(float delta)
{
	UpdateSystems(delta, m_physics_systems);
}
