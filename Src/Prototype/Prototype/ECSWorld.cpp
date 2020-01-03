#include "ECSWorld.h"

#include <SceneTree.hpp>
#include <KinematicBody.hpp>
#include <Node.hpp>
#include <Camera.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <PoolArrays.hpp>
#include <Navigation.hpp>
#include <CollisionShape.hpp>
#include <CapsuleShape.hpp>

#include "Components/Enemy.h"
#include "Components/AttackComponents.h"
#include "Components/InputComponents.h"
#include "Components/AIComponents/NavigationComponents.h"

#include "Systems/PlayerSystems/PlayerVelocitySystem.h"
#include "Systems/LocomotionSystems/KinematicMovementSystem.h"
#include "Systems/LocomotionSystems/GravitySystem.h";
#include "Systems/LocomotionSystems/JumpSystem.h";
#include "Systems/PlayerSystems/PlayerRotationSystem.h"
#include "Systems/SimpleFollowSystem.h"
#include "Systems/AttackSystems/MeleeAttackSystem.h"
#include "Systems/AttackSystems/CastAttackSystem.h"
#include "Systems/DestroyDeadSystem.h"
#include "Systems/PlayerSystems/PlayerInputSystem.h"
#include "Systems/WeaponChooseSystem.h"
#include "Systems/AttackSystems/ThrowAttackSystem.h"
#include "Systems/AttackSystems/ThrowableWeaponSystem.h"
#include "Systems/AISystems/NavAgentSystem.h"

#include "Components/Views/EntityView.h"

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
	EntityView* entityView = Object::cast_to<EntityView>(pPlayerNode->get_node("EntityView"));

	registry.assign<Node*>(entity, pPlayerNode);

	AssignNodeInheritedComponent<KinematicBody>(registry, entity, pPlayerNode);
	AssignNodeInheritedComponent<Spatial>(registry, entity, pPlayerNode);
	AssignNodeInheritedComponent<Camera>(registry, entity, get_node("Camera"));

	entityView->ConstructComponent(registry.assign<GravityComponent>(entity));
	entityView->ConstructComponent(registry.assign<JumpSpeedComponent>(entity));
	entityView->ConstructComponent(registry.assign<SpeedComponent>(entity));
	entityView->ConstructComponent(registry.assign<HealthComponent>(entity));

	WeaponHolderComponent weapons;
	entityView->ConstructComponent(weapons.melee);
	entityView->ConstructComponent(weapons.ranged);
	entityView->ConstructComponent(weapons.throwable);

	registry.assign<WeaponHolderComponent>(entity, weapons);
	registry.assign<MeleeAttackComponent>(entity, weapons.melee);
	//registry.assign<CastAttackComponent>(entity, weapons.ranged);
	//registry.assign<ThrowableAttackComponent>(entity, weapons.throwable);

	registry.assign<entt::tag<PlayerTag> >(entity);
	registry.assign<entt::tag<RotationTag> >(entity);
	registry.assign<entt::tag<PlayerInputTag> >(entity);

	registry.assign<VelocityComponent>(entity);
	registry.assign<InputComponent>(entity);
}

void godot::ECSWorld::PrepareCameraEntity()
{
	entt::entity entity = registry.create();

	Node* pCameraNode = get_node("Camera");
	AssignNodeInheritedComponent<Camera>(registry, entity, pCameraNode);
	AssignNodeInheritedComponent<Spatial>(registry, entity, get_node("Player"));

	EntityView* entityView = Object::cast_to<EntityView>(pCameraNode->get_node("EntityView"));
	entityView->ConstructComponent(registry.assign<SimpleFollowComponent>(entity));
}

void godot::ECSWorld::PrepareEnemyEntity()
{
	entt::entity entity = registry.create();

	Node* pEnemyNode = get_node("Enemy");
	EntityView* entityView = Object::cast_to<EntityView>(pEnemyNode->get_node("EntityView"));

	registry.assign<Node*>(entity, pEnemyNode);

	AssignNodeInheritedComponent<Spatial>(registry, entity, pEnemyNode);
	AssignNodeInheritedComponent<KinematicBody>(registry, entity, pEnemyNode);

	Enemy* pEnemy = AssignNodeInheritedComponent<Enemy>(registry, entity, pEnemyNode);
	pEnemy->SetEntity(entity);

	//TODO: should NavPathComponent and NavAgentComponent have views?
//<prepare NavPathComponent
	Navigation* nav = Object::cast_to<Navigation>(get_node("Navigation"));
	Node* pTargetNode = get_node("Navigation/NavigationMeshInstance/EnemyTarget");
	//TODO: in real life scenarios you should take targets bounds into account
	Vector3 target = Object::cast_to<Spatial>(pTargetNode)->get_global_transform().origin;
	PoolVector3Array path = nav->get_simple_path(pEnemy->get_transform().origin, target);
	registry.assign<NavPathComponent>(entity, path);
//prepare NavPathComponent>

//<prepare NavAgentComponent
	CollisionShape* colShape = Object::cast_to<CollisionShape>(pEnemyNode->get_node("CollisionShape"));
	Ref<Shape> shape = colShape->get_shape();
	CapsuleShape* capsuleShape = static_cast<CapsuleShape*>(shape.ptr());
	//cause capsule height is hieght of the cylinder
	float collisionOriginHeight = capsuleShape->get_radius() + capsuleShape->get_height() / 2;
	float minDistance = capsuleShape->get_margin();//margin used for physics, probably should increase min distance
	registry.assign<NavAgentComponent>(entity, collisionOriginHeight, capsuleShape->get_radius(), minDistance);
//prepare NavAgentComponent>
	
	entityView->ConstructComponent(registry.assign<SpeedComponent>(entity));
	entityView->ConstructComponent(registry.assign<HealthComponent>(entity));
	entityView->ConstructComponent(registry.assign<GravityComponent>(entity));

	registry.assign<VelocityComponent>(entity);
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
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new ThrowAttackSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new ThrowableWeaponSystem()));
	//TODO: should it be in phys proc?
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new NavAgentSystem()));
	
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
	//TODO: still not sure, maybe it have to be done as usual system and moved to process update
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
