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
#include <AnimationTree.hpp>

#include "Components/AttackComponents.h"
#include "Components/InputComponents.h"
#include "Components/AIComponents/NavigationComponents.h"
#include "Components/AIComponents/PatrolComponents.h"
#include "Components/AIComponents/FSMStateComponents.h"

#include "Systems/SystemChain.h"
#include "Systems/PlayerSystems/PlayerVelocitySystem.h"
#include "Systems/LocomotionSystems/KinematicMovementSystem.h"
#include "Systems/LocomotionSystems/GravitySystem.h";
#include "Systems/LocomotionSystems/JumpSystem.h";
#include "Systems/PlayerSystems/PlayerRotationSystem.h"
#include "Systems/SimpleFollowSystem.h"
#include "Systems/AttackSystems/MeleeAttackSystem.h"
#include "Systems/AttackSystems/RangedAttackSystem.h"
#include "Systems/DestroyDeadSystem.h"
#include "Systems/PlayerSystems/PlayerInputSystem.h"
#include "Systems/WeaponChooseSystem.h"
#include "Systems/AttackSystems/ThrowAttackSystem.h"
#include "Systems/AISystems/NavAgentSystem.h"
#include "Systems/LocomotionSystems/LookAtSystem.h"
#include "Systems/AISystems/PursuingSystem.h"
#include "Systems/AISystems/HealthMonitoringSystem.h"
#include "Systems/AISystems/FleeingSystem.h"
#include "Systems/AnimSystems/LocomotionAnimSystem.h"
#include "Systems/AnimSystems/HTHAnimSystem.h"
#include "Systems/AttackSystems/GrenadeSystem.h"
#include "Systems/AISystems/DecisionMakingFSMSystem.h"
#include "Systems/LocomotionSystems/HTHStuckSystem.h"

#include "Nodes/EntityHolderNode.h"

#include "Utils/Utils.h"

godot::ECSWorld* godot::ECSWorld::instance = nullptr;

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

	//TODO: check what node inherited components really should be assigned
	AssignNodeInheritedComponent<KinematicBody>(registry, entity, pPlayerNode);
	AssignNodeInheritedComponent<Spatial>(registry, entity, pPlayerNode);
	AssignNodeInheritedComponent<Camera>(registry, entity, get_node("Camera"));
	AssignNodeInheritedComponent<AnimationTree>(registry, entity, get_node("Player/vanguard/AnimationTree"));

	EntityHolderNode* pEntityHolder = AssignNodeInheritedComponent<EntityHolderNode>(registry, entity, pPlayerNode);
	pEntityHolder->SetEntity(entity);

	entityView->ConstructComponents(registry, entity);

	//TODO: add some kind of reactive callback to automatically assign curr weapon tag on assigning attack comp
	//		and remove manual tag assigning here and for bot too
	registry.assign<entt::tag<CurrentWeaponMeleeTag> >(entity);

	registry.assign<entt::tag<PlayerTag> >(entity);

	registry.assign<VelocityComponent>(entity);
	registry.assign<InputComponent>(entity);
	RotationDirectionComponent rot { registry.get<Spatial*>(entity)->get_global_transform().get_basis().z };
	registry.assign<RotationDirectionComponent>(entity, rot);

	registry.assign<BoundsComponent>(entity, utils::GetCapsuleBounds(pPlayerNode->get_node("CollisionShape")));
}

void godot::ECSWorld::PrepareCameraEntity()
{
	entt::entity entity = registry.create();

	Node* pCameraNode = get_node("Camera");
	AssignNodeInheritedComponent<Camera>(registry, entity, pCameraNode);
	AssignNodeInheritedComponent<Spatial>(registry, entity, pCameraNode);

	EntityView* entityView = Object::cast_to<EntityView>(pCameraNode->get_node("EntityView"));

	entityView->ConstructComponents(registry, entity);

	SimpleFollowComponent& followComp = registry.get<SimpleFollowComponent>(entity);

	followComp.targetEntity = Object::cast_to<EntityHolderNode>(get_node("Player"))->GetEntity();
	ASSERT(registry.valid(followComp.targetEntity), "invalid follow component target");

	registry.assign<entt::tag<MainCameraTag> >(entity);
}

void godot::ECSWorld::PrepareEnemyEntity()
{
	entt::entity entity = registry.create();

	Node* pEnemyNode = get_node("Enemy");
	EntityView* entityView = Object::cast_to<EntityView>(pEnemyNode->get_node("EntityView"));

	registry.assign<Node*>(entity, pEnemyNode);

	AssignNodeInheritedComponent<Spatial>(registry, entity, pEnemyNode);
	AssignNodeInheritedComponent<KinematicBody>(registry, entity, pEnemyNode);
	AssignNodeInheritedComponent<AnimationTree>(registry, entity, get_node("Enemy/vanguard/AnimationTree"));

	EntityHolderNode* pEntityHolder = AssignNodeInheritedComponent<EntityHolderNode>(registry, entity, pEnemyNode);
	pEntityHolder->SetEntity(entity);

	registry.assign<BoundsComponent>(entity, utils::GetCapsuleBounds(pEnemyNode->get_node("CollisionShape")));
	
	entityView->ConstructComponents(registry, entity);
	registry.assign<entt::tag<CurrentWeaponMeleeTag> >(entity);

	registry.assign<VelocityComponent>(entity);
	RotationDirectionComponent rot{ registry.get<Spatial*>(entity)->get_global_transform().get_basis().z };
	registry.assign<RotationDirectionComponent>(entity, rot);
	registry.assign<InputComponent>(entity);

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

	//initial fsm state
	registry.assign<entt::tag<PatrolStateTag> >(entity);
	
	registry.assign<entt::tag<BotTag> >(entity);
}

void godot::ECSWorld::PrepareSingletonEntities()
{
	ASSERT(registry.empty<Navigation*>(), "registry is not empty");

	Navigation* navigation = Object::cast_to<Navigation>(get_node("Navigation"));
	entt::entity navigationEntity = registry.create();
	registry.assign<Navigation*>(navigationEntity, navigation);
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
	InitInstance(this);

	utils::InitPhysicLayers();

	//TODO: try to move physics to separate thread
	//TODO: check what systems really should be in phys proc
	//TODO: check what systems should be reactive
	//setup physics systems
	m_physics_systems.emplace_back(new PlayerVelocitySystem());
	//m_physics_systems.emplace_back(new KinematicMovementSystem());
	m_physics_systems.emplace_back(new SystemChain<GravitySystem, JumpSystem>());
	m_physics_systems.emplace_back(new PlayerRotationSystem());
	m_physics_systems.emplace_back(new LookAtSystem());
	m_physics_systems.emplace_back(new MeleeAttackSystem());
	m_physics_systems.emplace_back(new RangedAttackSystem());
	m_physics_systems.emplace_back(new ThrowAttackSystem());

	//TODO: must be called after all systems that affects velocity (same line commented above)
	m_physics_systems.emplace_back(new KinematicMovementSystem());

	//TODO: make simple way to switch off bots (NavAgentSystem, PursuingSystem, FleeingSystem and others)
	m_physics_systems.emplace_back(new DecisionMakingFSMSystem(registry));

	m_physics_systems.emplace_back(new GrenadeSystem());
	//TODO: should it be in phys proc?
	m_physics_systems.emplace_back(new NavAgentSystem());
	m_physics_systems.emplace_back(new PursuingSystem());
	
	//setup systems
	m_process_systems.emplace_back(new SimpleFollowSystem());
	m_process_systems.emplace_back(new DestroyDeadSystem());
	m_process_systems.emplace_back(new WeaponChooseSystem(registry));
	m_process_systems.emplace_back(new HealthMonitoringSystem());
	m_process_systems.emplace_back(new FleeingSystem());
	m_process_systems.emplace_back(new LocomotionAnimSystem());
	m_process_systems.emplace_back(new HTHStuckSystem(registry));
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
	if (e->is_action_pressed("ui_accept"))
	{
		ResetInstance();
		registry.clear();
		PrepareSingletonEntities();
		get_tree()->reload_current_scene();
	}
	else if (e->is_action_pressed("ui_cancel"))
		get_tree()->quit();
	else
	{
		PlayerInputSystem inputSystem;
		inputSystem(registry, e);
	}
}

void godot::ECSWorld::_process(float delta)
{
	UpdateSystems(delta, m_process_systems);
}

void godot::ECSWorld::_physics_process(float delta)
{
	UpdateSystems(delta, m_physics_systems);
}