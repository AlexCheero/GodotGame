#include "ECSWorld.h"

#include <SceneTree.hpp>
#include <KinematicBody.hpp>
#include <Node.hpp>
#include <Camera.hpp>

#include "Components/Player.h"
#include "Components/Enemy.h"
#include "Components/AttackComponents.h"
#include "Components/InputComponents.h"

#include "Systems/PlayerVelocitySystem.h"
#include "Systems/KinematicMovementSystem.h"
#include "Systems/GravitySystem.h";
#include "Systems/JumpSystem.h";
#include "Systems/PlayerRotationSystem.h"
#include "Systems/SimpleFollowSystem.h"
#include "Systems/MeleeAttackSystem.h"
#include "Systems/CastAttackSystem.h"
#include "Systems/DestroyDeadSystem.h"
#include "Systems/PlayerInputSystem.h"
#include "Systems/WeaponChooseSystem.h"

#include "Utils.h"

void godot::ECSWorld::UpdateSystems(float delta, SystemsVec& systems)
{
	for (auto& system : systems)
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

	registry.assign<GravityComponent>(entity, 30.f, 20.f);
	registry.assign<JumpSpeedComponent>(entity, 30.f);//TODO: find a way to set these values via editor
	registry.assign<entt::tag<RotationTag> >(entity);
	registry.assign<VelocityComponent>(entity);
	registry.assign<SpeedComponent>(entity, 30.f);
	registry.assign<HealthComponent>(entity, 100.f);

	WeaponHolderComponent weapons;
	weapons.melee = MelleAttackComponent{ 4.f, 10.f, 90.f, 0.5f };
	weapons.ranged = CastAttackComponent{ 40.f, 50.f, 0.5f };
	registry.assign<WeaponHolderComponent>(entity, weapons);
	registry.assign<MelleAttackComponent>(entity, weapons.melee);
	//registry.assign<CastAttackComponent>(entity, weapons.ranged);

	registry.assign<entt::tag<PlayerInputTag> >(entity);

	registry.assign<InputComponent>(entity);
}

void godot::ECSWorld::PrepareCameraEntity()
{
	entt::entity entity = registry.create();

	Camera* pCamera = Object::cast_to<Camera>(get_node("Camera"));
	registry.assign<Camera*>(entity, pCamera);
	registry.assign<SimpleFollowComponent>(entity, 15.f, -30.f, -45.f);
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
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new PlayerVelocitySystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new KinematicMovementSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new GravitySystem()));
	//TODO: must always follow GravitySystem. find a way to enforce such behaviour in entt
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new JumpSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new PlayerRotationSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new MeleeAttackSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new CastAttackSystem()));
	
	//setup systems
	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new SimpleFollowSystem()));
	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new DestroyDeadSystem()));
	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new WeaponChooseSystem()));
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
	PlayerInputSystem inputSystem;
	inputSystem(registry, e);

	if (e->is_action_pressed("ui_accept"))
	{
		registry.reset();
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
