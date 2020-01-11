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

#include "Components/NodeComponents/EnemyNodeComponent.h"
#include "Components/AttackComponents.h"
#include "Components/InputComponents.h"
#include "Components/AIComponents/NavigationComponents.h"
#include "Components/AIComponents/PatrolComponents.h"

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
#include "Systems/LocomotionSystems/LookAtSystem.h"
#include "Systems/AISystems/PatrolSystem.h"
#include "Systems/AISystems/PursuingSystem.h"

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
	registry.assign<entt::tag<PlayerInputTag> >(entity);

	registry.assign<VelocityComponent>(entity);
	registry.assign<InputComponent>(entity);
	registry.assign<RotationDirectionComponent>(entity);

	registry.assign<BoundsComponent>(entity, GetCapsuleBounds(pPlayerNode->get_node("CollisionShape")));
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

	EnemyNodeComponent* pEnemy = AssignNodeInheritedComponent<EnemyNodeComponent>(registry, entity, pEnemyNode);
	pEnemy->SetEntity(entity);

	registry.assign<BoundsComponent>(entity, GetCapsuleBounds(pEnemyNode->get_node("CollisionShape")));
	
	entityView->ConstructComponent(registry.assign<SpeedComponent>(entity));
	entityView->ConstructComponent(registry.assign<HealthComponent>(entity));
	entityView->ConstructComponent(registry.assign<GravityComponent>(entity));
	entityView->ConstructComponent(registry.assign<PatrolmanComponent>(entity));
	entityView->ConstructComponent(registry.assign<NavMarginComponent>(entity));

	registry.assign<VelocityComponent>(entity);
	registry.assign<RotationDirectionComponent>(entity);

//<prepare patrol route
	PatrolRouteComponent& route = registry.assign<PatrolRouteComponent>(entity);
	route.routePoints.clear();

	Node* pPatrolRoute = get_node("PatrolRoute");
	for (int i = 0; i < pPatrolRoute->get_child_count(); i++)
	{
		Spatial* spatialChild = Object::cast_to<Spatial>(pPatrolRoute->get_child(i));
		if (!spatialChild)
			continue;
		
		route.routePoints.push_back(spatialChild->get_global_transform().origin);
	}
	route.current = 0;
//prepare patrol route>

	registry.assign<entt::tag<PatrollingTag> >(entity);
}

void godot::ECSWorld::PrepareSingletonEntities()
{
	Navigation* navigation = Object::cast_to<Navigation>(get_node("Navigation"));
	entt::entity navigationEntity = registry.create();
	registry.assign<Navigation*>(navigationEntity, navigation);
}

godot::BoundsComponent godot::ECSWorld::GetCapsuleBounds(Node* pCapsuleNode)
{
	CollisionShape* colShape = Object::cast_to<CollisionShape>(pCapsuleNode);
	Ref<Shape> shape = colShape->get_shape();
	CapsuleShape* capsuleShape = static_cast<CapsuleShape*>(shape.ptr());
	//cause capsule height is hieght of the cylinder
	float capsuleRadius = capsuleShape->get_radius();
	float boundsHeight = 2 * capsuleRadius + capsuleShape->get_height();
	float boundsWidth, boundsLength;
	boundsWidth = boundsLength = capsuleRadius * 2;
	//margin used for physics, probably should increase min distance
	return { boundsWidth, boundsHeight, boundsLength, capsuleShape->get_margin() };
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

	//TODO: check what systems really should be in phys proc
	//setup physics systems
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new PlayerVelocitySystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new KinematicMovementSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new GravitySystem()));
	//TODO: must always follow GravitySystem. find a way to enforce such behaviour in entt
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new JumpSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new PlayerRotationSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new LookAtSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new MeleeAttackSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new CastAttackSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new ThrowAttackSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new ThrowableWeaponSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new PatrolSystem()));
	//TODO: should it be in phys proc?
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new NavAgentSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new PursuingSystem()));
	
	//setup systems
	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new SimpleFollowSystem()));
	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new DestroyDeadSystem()));
	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new WeaponChooseSystem()));
}

void godot::ECSWorld::_ready()
{
	//create entities and components
	PrepareSingletonEntities();
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
		//TODO: fix reset
		registry.reset();
		PrepareSingletonEntities();
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
