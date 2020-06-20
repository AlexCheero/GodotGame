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

//TODO: move E CS_EVENTS macro to separate header or split meta types and type enumerations
//      into separate headers
#include "Components/RegisteredTypes.h"

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
#include "Systems/PlayerSystems/PlayerAttackInputSystem.h"

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

#include "Systems/AnimSystems/LocomotionAnimSystem.h"
#include "Systems/AnimSystems/EndAttackAnimSystem.h"

#include "Utils/Utils.h"

godot::ECSWorld* godot::ECSWorld::instance = nullptr;

template<typename T>
T* godot::ECSWorld::AssignNodeInheritedComponent(entt::registry& registry, entt::entity entity, Node* pNode)
{
	T* pComp = Object::cast_to<T>(pNode);
	return registry.emplace<T*>(entity, pComp);
}

template<typename Type, typename ...Types>
inline void godot::ECSWorld::PrepareEcsEventsClearingSystems(SystemsVec& systems)
{
	systems.emplace_back([](float delta, entt::registry& registry)
		{ auto view = registry.view<Type>(); registry.remove<Type>(view.begin(), view.end()); }
	);
	//TODO: think about how to make this without templates to avoid code bloating
	//      use fold expressions like: 
	/*
	template <typename ...Types>
    void DoSmthForAllTypes()
    {
      (DoSmthForType<Types>(), ...);
    }
	*/
	if constexpr (sizeof...(Types))
		PrepareEcsEventsClearingSystems<Types...>(systems);
}

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

	registry.emplace<MeleeAttackComponent>(entity).hits = LoadHits("barehanded_hits");
	registry.emplace<AttackCooldownComponent>(entity);

	registry.emplace<VelocityComponent>(entity);
	
	//TODO: probably assign all input components at once in method or something
	registry.emplace<RotationInputComponent>(entity);
	registry.emplace<MoveDirInputComponent>(entity);
	registry.emplace<AttackInputComponent>(entity);
	registry.emplace<AttackInputAggregatorComponent>(entity);

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

	registry.emplace<MeleeAttackComponent>(entity).hits = LoadHits("barehanded_hits");
	registry.emplace<AttackCooldownComponent>(entity);

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

std::vector<float> godot::PlayerAttackInputSystem::angles;
int64_t godot::PlayerAttackInputSystem::patternMatchingTime;

void godot::ECSWorld::LoadConfig()
{
	ConfigFile* hitsCfg = ConfigFile::_new();
	Error err = hitsCfg->load("res://Configs/config.cfg");
	ASSERT(err == Error::OK, "cannot load config.cfg");
	maxComboIntervalMillis = hitsCfg->get_value("maxComboIntervalMillis", "maxComboIntervalMillis");

	Array angles = hitsCfg->get_value("meleeAttackInputPatternsAngles", "angles");
	for (int i = 0; i < angles.size(); i++)
		PlayerAttackInputSystem::angles.push_back(angles[i]);

	PlayerAttackInputSystem::patternMatchingTime = hitsCfg->get_value("meleeAttackInputPatternMatchingTime", "msec");

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

std::vector<MeleeHit> MeleeAttackComponent::hitsData;

void godot::ECSWorld::LoadMeleeAttacksConfig()
{
	MeleeAttackComponent::hitsData.clear();

	ConfigFile* hitsCfg = ConfigFile::_new();
	Error err = hitsCfg->load("res://Configs/melee_attack.cfg");
	ASSERT(err == Error::OK, "cannot load melee attack config");
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

		Array arr = hitsCfg->get_value(section, "pattern");
		for (int i = 0; i < arr.size(); i++)
			hit.inputPattern.push_back(arr[i]);

		MeleeAttackComponent::hitsData.push_back(hit);
	}
	hitsCfg->free();
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
	//Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);

	InitInstance(this);

	//TODO: do only once, not after every reset
	utils::InitPhysicLayers();
	LoadMeleeAttacksConfig();

//setup reactive systems
	//TODO: probably make registration and auto init of all initable systems via registartor
	//      or implement emplacing method that will Init systems automatically
	WeaponChooseRSystem::Init(registry);
	
//setup physics systems
	physics_systems.emplace_back(GravitySystem::Tick);
	//must be called after all systems that affects velocity
	physics_systems.emplace_back(KinematicMovementSystem::Tick);
	
//setup systems
	//produce input, should be placed before any input related system
	//comment to switch off bots
	DecisionMakingFSMSystem::Init(registry);
	process_systems.emplace_back(DecisionMakingFSMSystem::Tick);

	process_systems.emplace_back(JumpSystem::Tick);
	process_systems.emplace_back(PlayerVelocitySystem::Tick);
	process_systems.emplace_back(PlayerRotationSystem::Tick);
	process_systems.emplace_back(PlayerAttackInputSystem::Tick);

//<melee systems
	process_systems.emplace_back(MeleeAttackCooldownSystem::Tick);
	
	MeleeLockTargetSystem::Init();
	process_systems.emplace_back(MeleeLockTargetSystem::Tick);

	process_systems.emplace_back(UpdateLockRotationSystem::Tick);
	//TODO_melee: implement proper melee with blocks and stuff
	process_systems.emplace_back(MeleeAnimSystem::Tick);
	process_systems.emplace_back(CheckForPileInSystem::Tick);

	PileInSystem::Init(registry);
	process_systems.emplace_back(PileInSystem::Tick);

	ComboDropSystem::Init(registry);
	process_systems.emplace_back(ComboDropSystem::Tick);

	process_systems.emplace_back(IncrementComboSystem::Tick);
	process_systems.emplace_back(EndAttackAnimSystem::Tick);
//melee systems>
	
	process_systems.emplace_back(RangedAttackSystem::Tick);
	process_systems.emplace_back(ThrowAttackSystem::Tick);
	process_systems.emplace_back(ThrowGrenadeSystem::Tick);

	GrenadeSystem::Init();
	process_systems.emplace_back(GrenadeSystem::Tick);

	process_systems.emplace_back(NavAgentSystem::Tick);
	process_systems.emplace_back(PursuingSystem::Tick);
	
	process_systems.emplace_back(LookAtSystem::Tick);
	process_systems.emplace_back(DestroyDeadSystem::Tick);
	process_systems.emplace_back(HealthMonitoringSystem::Tick);
	process_systems.emplace_back(FleeingSystem::Tick);
	process_systems.emplace_back(LocomotionAnimSystem::Tick);

	//TODO0: clear events before system where it was assigned, not in the end of frame
	PrepareEcsEventsClearingSystems<ECS_EVENTS>(process_systems);

	//TODO0: remove when parameterized events will bew implemented
	process_systems.emplace_back([](float delta, entt::registry& registry)
	{
		auto view = registry.view<MeleeAttackParameterizedEvent>();
		view.each([&registry](entt::entity entity, MeleeAttackParameterizedEvent evt)
		{
			registry.remove_if_exists<MeleeAttackParameterizedEvent>(entity);
		});
	});
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
		DeleteInstance();
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