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

#include <Input.hpp>

#include "Components/AttackComponents.h"
#include "Components/InputComponents.h"
#include "Components/AIComponents/NavigationComponents.h"
#include "Components/AIComponents/PatrolComponents.h"
#include "Components/AIComponents/FSMStateComponents.h"

#include "ReactiveSystems/WeaponChooseRSystem.h"

#include "Systems/AttackSystems/UpdateLockRotationSystem.h"
#include "Systems/AttackSystems/GrenadeSystem.h"
#include "Systems/AttackSystems/RangedAttackSystem.h"
#include "Systems/AttackSystems/ThrowAttackSystem.h"
#include "Systems/AttackSystems/ThrowGrenadeSystem.h"

#include "Systems/AttackSystems/Melee/MeleeAnimSystem.h"
#include "Systems/AttackSystems/Melee/MeleeAttackCooldownSystem.h"
#include "Systems/AttackSystems/Melee/MeleeLockTargetSystem.h"
#include "Systems/AttackSystems/Melee/CheckForPileInSystem.h"
#include "Systems/AttackSystems/Melee/IncrementComboSystem.h"
#include "Systems/AttackSystems/Melee/PileInSystem.h"
#include "Systems/AttackSystems/Melee/ComboDropSystem.h"

#include "Systems/PlayerSystems/PlayerInputSystem.h"
#include "Systems/PlayerSystems/PlayerVelocitySystem.h"
#include "Systems/PlayerSystems/PlayerRotationSystem.h"

#include "Systems/LocomotionSystems/KinematicMovementSystem.h"
#include "Systems/LocomotionSystems/GravitySystem.h";
#include "Systems/LocomotionSystems/LookAtSystem.h"
#include "Systems/LocomotionSystems/JumpSystem.h"

#include "Systems/DestroyDeadSystem.h"

#include "Systems/AISystems/NavAgentSystem.h"
#include "Systems/AISystems/PursuingSystem.h"
#include "Systems/AISystems/HealthMonitoringSystem.h"
#include "Systems/AISystems/FleeingSystem.h"
#include "Systems/AISystems/DecisionMaking/DecisionMakingFSMSystem.h"
#include "Systems/AISystems/ClearBotInputSystem.h"

#include "Systems/AnimSystems/LocomotionAnimSystem.h"
#include "Systems/AnimSystems/EndAttackAnimSystem.h"

#include "Utils/Utils.h"

godot::ECSWorld* godot::ECSWorld::instance = nullptr;

void godot::ECSWorld::UpdateSystems(float delta, SystemsVec& systems)
{
	for (auto& system : systems)
		system(delta, registry);
}

void godot::ECSWorld::PreparePlayerEntity()
{
	entt::entity entity = registry.create();

	//TODO: use node paths and remove hardcodes. separate abstract ECSWorld from level specific. and move all node paths there
	Node* pPlayerNode = get_node("Player");
	EntityView* entityView = Object::cast_to<EntityView>(pPlayerNode->get_node("EntityView"));
	entityView->SetEntity(entity);

	registry.emplace<Node*>(entity, pPlayerNode);

	AssignNodeInheritedComponent<KinematicBody>(registry, entity, pPlayerNode);
	AssignNodeInheritedComponent<Spatial>(registry, entity, pPlayerNode);
	//TODO: refactor camera GDScript, use interpolated camera and check why spring arm doesn't works properly
	AssignNodeInheritedComponent<Camera>(registry, entity, get_node("CameraGimbal/InnerGimbal/SpringArm/Camera"));
	AssignNodeInheritedComponent<AnimationTree>(registry, entity, get_node("Player/vanguard/AnimationTree"));

	entityView->ConstructComponents(registry, entity);
	entityView->ConstructTags(registry, entity);

	registry.get<MeleeAttackComponent>(entity).hits = LoadHits("barehanded_hits");

	registry.emplace<VelocityComponent>(entity);
	
	registry.emplace<RotationInputComponent>(entity);
	registry.emplace<MoveDirInputComponent>(entity);

	RotationDirectionComponent rot { registry.get<Spatial*>(entity)->get_global_transform().get_basis().z };
	registry.emplace<RotationDirectionComponent>(entity, rot);

	registry.emplace<BoundsComponent>(entity, utils::GetCapsuleBounds(pPlayerNode->get_node("CollisionShape")));
}

void godot::ECSWorld::PrepareEnemyEntity()
{
	entt::entity entity = registry.create();

	Node* pEnemyNode = get_node("Enemy");
	EntityView* entityView = Object::cast_to<EntityView>(pEnemyNode->get_node("EntityView"));
	entityView->SetEntity(entity);

	registry.emplace<Node*>(entity, pEnemyNode);

	AssignNodeInheritedComponent<Spatial>(registry, entity, pEnemyNode);
	AssignNodeInheritedComponent<KinematicBody>(registry, entity, pEnemyNode);
	AssignNodeInheritedComponent<AnimationTree>(registry, entity, get_node("Enemy/vanguard/AnimationTree"));

	registry.emplace<BoundsComponent>(entity, utils::GetCapsuleBounds(pEnemyNode->get_node("CollisionShape")));

	entityView->ConstructComponents(registry, entity);
	entityView->ConstructTags(registry, entity);

	registry.get<MeleeAttackComponent>(entity).hits = LoadHits("barehanded_hits");

	registry.emplace<VelocityComponent>(entity);
	RotationDirectionComponent rot{ registry.get<Spatial*>(entity)->get_global_transform().get_basis().z };
	registry.emplace<RotationDirectionComponent>(entity, rot);
	
	//commented because enemy probably don't need this
	//registry.assign<RotationInputComponent>(entity);
	//registry.assign<MoveDirInputComponent>(entity);

//<prepare patrol route
	PatrolRouteComponent& route = registry.emplace<PatrolRouteComponent>(entity);
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
	registry.emplace<PatrolStateTag>(entity);
}

void godot::ECSWorld::PrepareSingletonEntities()
{
	ASSERT(registry.empty<Navigation*>(), "registry is not empty");

	Navigation* navigation = Object::cast_to<Navigation>(get_node("Navigation"));
	entt::entity navigationEntity = registry.create();
	registry.emplace<Navigation*>(navigationEntity, navigation);
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

void godot::ECSWorld::_init()
{
	//TODO: move to more appropriate place (with #include <Input.hpp>)
	Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);

	InitInstance(this);

	utils::InitPhysicLayers();

	InitSystems
		<
			WeaponChooseRSystem,
			MeleeLockTargetSystem,
			PileInSystem,
			ComboDropSystem,
			DecisionMakingFSMSystem,
			GrenadeSystem
		>(registry);

//setup physics systems
	EmplaceSystems
		<
			GravitySystem,
			KinematicMovementSystem //must be called after all systems that affects velocity
		>(physics_systems);
//setup systems

	EmplaceSystems
		<
			JumpSystem,
		    PlayerVelocitySystem,
		    PlayerRotationSystem,

//<melee systems
            MeleeAttackCooldownSystem,
            MeleeLockTargetSystem,
            UpdateLockRotationSystem,
            //TODO_melee: implement proper melee with blocks and stuff
            MeleeAnimSystem,
            CheckForPileInSystem,
            PileInSystem,
            ComboDropSystem,
            IncrementComboSystem,
            EndAttackAnimSystem,
//melee systems>

            RangedAttackSystem,
            ThrowAttackSystem,
	        ThrowGrenadeSystem,
		    //place before any ai systems
		    ClearBotInputSystem,
		    //comment to switch off bots
		    DecisionMakingFSMSystem,

		    GrenadeSystem,

		    NavAgentSystem,
		    PursuingSystem,

		    LookAtSystem,
		    DestroyDeadSystem,
			HealthMonitoringSystem,
			FleeingSystem,
			LocomotionAnimSystem
		>(process_systems);
}

void godot::ECSWorld::_ready()
{
	LoadConfig();
	//create entities and components
	PrepareSingletonEntities();
	PreparePlayerEntity();
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
		 PlayerInputSystem::HandleInput(registry);
	
}

void godot::ECSWorld::_process(float delta)
{
	UpdateSystems(delta, process_systems);
}

void godot::ECSWorld::_physics_process(float delta)
{
	UpdateSystems(delta, physics_systems);
}