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
#include <ConfigFile.hpp>

#include "Components/AttackComponents.h"
#include "Components/InputComponents.h"
#include "Components/AIComponents/NavigationComponents.h"
#include "Components/AIComponents/PatrolComponents.h"
#include "Components/AIComponents/FSMStateComponents.h"

#include "ReactiveSystems/JumpRSystem.h";

#include "Systems/PlayerSystems/PlayerVelocitySystem.h"
#include "Systems/LocomotionSystems/KinematicMovementSystem.h"
#include "Systems/LocomotionSystems/GravitySystem.h";
#include "Systems/PlayerSystems/PlayerRotationSystem.h"
#include "Systems/SimpleFollowSystem.h"
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
#include "Systems/AISystems/DecisionMaking/DecisionMakingFSMSystem.h"

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
	entityView->SetEntity(entity);

	registry.assign<Node*>(entity, pPlayerNode);

	AssignNodeInheritedComponent<KinematicBody>(registry, entity, pPlayerNode);
	AssignNodeInheritedComponent<Spatial>(registry, entity, pPlayerNode);
	AssignNodeInheritedComponent<Camera>(registry, entity, get_node("Camera"));
	AssignNodeInheritedComponent<AnimationTree>(registry, entity, get_node("Player/vanguard/AnimationTree"));

	registry.assign<MeleeAttackComponent>(entity, MeleeAttackComponent{ LoadHits("barehanded_hits") });

	entityView->ConstructComponents(registry, entity);
	entityView->ConstructTags(registry, entity);

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
	entityView->ConstructTags(registry, entity);

	SimpleFollowComponent& followComp = registry.get<SimpleFollowComponent>(entity);

	followComp.targetEntity = Object::cast_to<EntityView>(get_node("Player")->get_node("EntityView"))->GetEntity();
	ASSERT(registry.valid(followComp.targetEntity), "invalid follow component target");
}

void godot::ECSWorld::PrepareEnemyEntity()
{
	entt::entity entity = registry.create();

	Node* pEnemyNode = get_node("Enemy");
	EntityView* entityView = Object::cast_to<EntityView>(pEnemyNode->get_node("EntityView"));
	entityView->SetEntity(entity);

	registry.assign<Node*>(entity, pEnemyNode);

	AssignNodeInheritedComponent<Spatial>(registry, entity, pEnemyNode);
	AssignNodeInheritedComponent<KinematicBody>(registry, entity, pEnemyNode);
	AssignNodeInheritedComponent<AnimationTree>(registry, entity, get_node("Enemy/vanguard/AnimationTree"));

	registry.assign<BoundsComponent>(entity, utils::GetCapsuleBounds(pEnemyNode->get_node("CollisionShape")));
	
	registry.assign<MeleeAttackComponent>(entity, MeleeAttackComponent{ LoadHits("barehanded_hits") });

	entityView->ConstructComponents(registry, entity);
	entityView->ConstructTags(registry, entity);

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
	registry.assign<PatrolStateTag>(entity);
}

void godot::ECSWorld::PrepareSingletonEntities()
{
	ASSERT(registry.empty<Navigation*>(), "registry is not empty");

	Navigation* navigation = Object::cast_to<Navigation>(get_node("Navigation"));
	entt::entity navigationEntity = registry.create();
	registry.assign<Navigation*>(navigationEntity, navigation);
}

int64_t MeleeAttackComponent::maxComboIntervalMillis;
void godot::ECSWorld::LoadConfig()
{
	ConfigFile* hitsCfg = ConfigFile::_new();
	Error err = hitsCfg->load("res://Configs/config.cfg");
	ASSERT(err == Error::OK, "cannot load config.cfg");
	MeleeAttackComponent::maxComboIntervalMillis = hitsCfg->get_value("maxComboIntervalMillis", "maxComboIntervalMillis");
	hitsCfg->free();
}

//TODO: maybe cache config and not load it every time
std::vector<MeleeHit> godot::ECSWorld::LoadHits(String hitsConfigName)
{
	std::vector<MeleeHit> hits;
	ConfigFile* hitsCfg = ConfigFile::_new();
	Error err = hitsCfg->load("res://Configs/hits/" + hitsConfigName + ".cfg");
	ASSERT(err == Error::OK, "cannot load hits config");
	PoolStringArray sections = hitsCfg->get_sections();
	for (int i = 0; i < sections.size(); i++)
	{
		String section = sections[i];
		MeleeHit hit =
		{
			section,
			hitsCfg->get_value(section, "damage"),
			hitsCfg->get_value(section, "attackTime"),
			hitsCfg->get_value(section, "maxDistance"),
			hitsCfg->get_value(section, "minDistance")
		};
		hits.push_back(hit);
	}
	hitsCfg->free();

	return hits;
}

void godot::ECSWorld::_register_methods()
{
	register_method((char*)"_init", &ECSWorld::_init);
	register_method((char*)"_ready", &ECSWorld::_ready);
	register_method((char*)"_input", &ECSWorld::HandleInputEvent);
	register_method((char*)"_process", &ECSWorld::_process);
	register_method((char*)"_physics_process", &ECSWorld::_physics_process);
}

//TODO0: move includes upwards
#include "ReactiveSystems/StartMeleeAttackRSystem.h"
#include "ReactiveSystems/HTHLockTargetSystem.h"
#include "Systems/AttackSystems/PileInSystem.h"
#include "Systems/AttackSystems/IncrementComboSystem.h"
#include "Systems/AttackSystems/UpdateLockRotationSystem.h"
#include "Systems/AnimSystems/EndAttackAnimSystem.h"
#include "Systems/AttackSystems/CheckForPileInSystem.h"

void godot::ECSWorld::_init()
{
	InitInstance(this);

	utils::InitPhysicLayers();

	//TODO: check what systems should be reactive
	//setup reactive systems
	JumpRSystem::Init(registry);
	StartMeleeAttackRSystem::Init(registry);
	HTHLockTargetRSystem::Init(registry);

	//setup physics systems
	m_physics_systems.emplace_back(new GravitySystem());
	//must be called after GravitySystem
	//must be called after all systems that affects velocity
	m_physics_systems.emplace_back(new KinematicMovementSystem());
	
	//setup systems

	//TODO: implement proper hth with blocks and stuff
	//TODO0: make some parts reactive
//<MeleeAttackSystem
	//TODO: locks on target on every hit, this may cause bugs with many enemies
	m_process_systems.emplace_back(new UpdateLockRotationSystem());
	m_process_systems.emplace_back(new HTHAnimSystem()); //reactive
	m_process_systems.emplace_back(new EndAttackAnimSystem());
	m_process_systems.emplace_back(new CheckForPileInSystem()); //reactive
	m_process_systems.emplace_back(new PileInSystem());
	m_process_systems.emplace_back(new IncrementComboSystem()); //reactive
//MeleeAttackSystem>
	
	m_process_systems.emplace_back(new RangedAttackSystem()); //reactive
	m_process_systems.emplace_back(new ThrowAttackSystem()); //reactive

	//comment to switch off bots
	m_process_systems.emplace_back(new DecisionMakingFSMSystem(registry));

	m_process_systems.emplace_back(new GrenadeSystem());
	m_process_systems.emplace_back(new NavAgentSystem()); //check better for reactivness
	m_process_systems.emplace_back(new PursuingSystem());
	
	//TODO: pile in breaks if this system runs in _process
	m_physics_systems.emplace_back(new PlayerVelocitySystem()); //reactive
	m_process_systems.emplace_back(new PlayerRotationSystem()); //reactive
	m_process_systems.emplace_back(new LookAtSystem());
	m_process_systems.emplace_back(new SimpleFollowSystem());
	m_process_systems.emplace_back(new DestroyDeadSystem()); //reactive?
	m_process_systems.emplace_back(new WeaponChooseSystem(registry)); //reactive
	m_process_systems.emplace_back(new HealthMonitoringSystem()); //reactive?
	m_process_systems.emplace_back(new FleeingSystem());
	m_process_systems.emplace_back(new LocomotionAnimSystem());
}

void godot::ECSWorld::_ready()
{
	LoadConfig();
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
		 PlayerInputSystem::HandleInput(registry, e);
	
}

void godot::ECSWorld::_process(float delta)
{
	UpdateSystems(delta, m_process_systems);
}

void godot::ECSWorld::_physics_process(float delta)
{
	UpdateSystems(delta, m_physics_systems);
}