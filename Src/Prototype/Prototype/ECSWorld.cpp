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

#include "Components/NodeComponents/Animation2DComponent.h"
#include "Components/AttackComponents.h"
#include "Components/InputComponents.h"
#include "Components/AIComponents/NavigationComponents.h"
#include "Components/AIComponents/PatrolComponents.h"
#include "Components/AIComponents/FSMStateComponents.h"

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
#include "Systems/AISystems/PatrolSystem.h"
#include "Systems/AISystems/PursuingSystem.h"
#include "Systems/AISystems/LookAroundSystem.h"
#include "Systems/AISystems/HealthMonitoringSystem.h"
#include "Systems/AISystems/FleeingSystem.h"
#include "Systems/BillboardRotationSystem.h"

#include "Nodes/EntityHolderNode.h"

#include "Utils/Utils.h"

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
	AssignNodeInheritedComponent<Animation2DComponent>(registry, entity, get_node("Player/Sprite3D"));

	EntityHolderNode* pEnemy = AssignNodeInheritedComponent<EntityHolderNode>(registry, entity, pPlayerNode);
	pEnemy->SetEntity(entity);

	entityView->ConstructComponents(registry, entity);

	//TODO: add some kind of reactive callback to automatically assign curr weapon tag on assigning attack comp
	//		and remove manual tag assigning here and for bot too
	registry.assign<entt::tag<CurrentWeaponMeleeTag> >(entity);

	registry.assign<entt::tag<PlayerTag> >(entity);

	registry.assign<VelocityComponent>(entity);
	registry.assign<InputComponent>(entity);
	registry.assign<RotationDirectionComponent>(entity);

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
	AssignNodeInheritedComponent<Animation2DComponent>(registry, entity, get_node("Enemy/Sprite3D"));

	EntityHolderNode* pEnemy = AssignNodeInheritedComponent<EntityHolderNode>(registry, entity, pEnemyNode);
	pEnemy->SetEntity(entity);

	registry.assign<BoundsComponent>(entity, utils::GetCapsuleBounds(pEnemyNode->get_node("CollisionShape")));
	
	entityView->ConstructComponents(registry, entity);
	registry.assign<entt::tag<CurrentWeaponMeleeTag> >(entity);

	registry.assign<VelocityComponent>(entity);
	registry.assign<RotationDirectionComponent>(entity);
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

	registry.assign<entt::tag<PatrollingTag> >(entity);
	registry.assign<entt::tag<BotTag> >(entity);
}

void godot::ECSWorld::PrepareSingletonEntities()
{
	//TODO: find out why registry is empty and use assert if it will fit
	if (registry.empty<Navigation*>())
	{
		Navigation* navigation = Object::cast_to<Navigation>(get_node("Navigation"));
		entt::entity navigationEntity = registry.create();
		registry.assign<Navigation*>(navigationEntity, navigation);
	}
	else
		Godot::print_warning("trying to assign more than one singleton entity", "PrepareSingletonEntities", "ECSWorld.cpp", __LINE__);
}

//TODO: think about refactoring all the callbacks for signals (moving it somwhere out of world)
void godot::ECSWorld::_on_Pickable_picked_up(Node* pPicker, EntityView* pPickableView, int pickableType)
{
	EntityHolderNode* pPickerEntityHolder = Object::cast_to<EntityHolderNode>(pPicker);
	if (!pPickerEntityHolder)
		return;

	entt::entity pickerEntity = pPickerEntityHolder->GetEntity();
	ASSERT(pickerEntity != entt::null, "picker is null");
	ASSERT(pPickableView != nullptr, "pickable view is null");
	//TODO: pick/change weapon by button press
	//		and probably not switch on pickup
	EPickableType pickableEnmVal = static_cast<EPickableType>(pickableType);
	switch (pickableEnmVal)
	{
	case EPickableType::MeleeWeapon:
	{
		bool constructed = pPickableView->ConstructComponent(registry.assign_or_replace<MeleeAttackComponent>(pickerEntity));
		ASSERT(constructed, "can't construct MeleeAttackComponent");
		//if switch on pickup
		registry.get_or_assign<entt::tag<CurrentWeaponMeleeTag> >(pickerEntity);
		break;
	}
	case EPickableType::RangedWeapon:
	{
		bool constructed = pPickableView->ConstructComponent(registry.assign_or_replace<RangedAttackComponent>(pickerEntity));
		ASSERT(constructed, "can't construct RangedAttackComponent");
		//if switch on pickup
		registry.get_or_assign<entt::tag<CurrentWeaponRangedTag> >(pickerEntity);
		break;
	}
	case EPickableType::ThrowableWeapon:
	{
		bool constructed = pPickableView->ConstructComponent(registry.assign_or_replace<ThrowableAttackComponent>(pickerEntity));
		ASSERT(constructed, "can't construct ThrowableAttackComponent");
		//if switch on pickup
		registry.get_or_assign<entt::tag<CurrentWeaponThrowableTag> >(pickerEntity);
		break;
	}
	case EPickableType::Medkit:
		Godot::print("Picked up Medkit");
		break;
	case EPickableType::Buff:
		Godot::print("Picked up Buff");
		break;
	case EPickableType::Key:
		Godot::print("Picked up Key");
		break;
	default:
		Godot::print_error("Wrong pickable type: " + String::num_int64(pickableType), "_on_Pickable_picked_up", "ECSWorld", __LINE__);
		break;
	}
}

void godot::ECSWorld::_on_Throwable_hit(Node* pTarget, ThrowableWeaponNode* pThrowable)
{
	Godot::print("throwable hit");
	EntityHolderNode* pTargetEntityHolder = Object::cast_to<EntityHolderNode>(pTarget);
	if (!pTargetEntityHolder)
		return;

	entt::entity hittedEntity = pTargetEntityHolder->GetEntity();

	//TODO: probably should check (or assert?) registry.has<entt::tag<DeadTag> >(hittedEntity)
	ASSERT(hittedEntity != entt::null, "picker is null");
	ASSERT(registry.has<HealthComponent>(hittedEntity), "hitted entity has no HealthComponent");
	ASSERT(pThrowable != nullptr, "pickable view is null");

	HealthComponent& health = registry.get<HealthComponent>(hittedEntity);

	health.hp -= pThrowable->GetDamage();
}

void godot::ECSWorld::_on_Grenade_explosion(Node* pTarget, GrenadeNode* pGrenade)
{
	for (int i = 0; i < pGrenade->GetHitted().size(); i++)
	{
		EntityHolderNode* pEntitiyHolder = Object::cast_to<EntityHolderNode>(pGrenade->GetHitted()[i]);
		ASSERT(pEntitiyHolder != nullptr, "entity holder hitted by explosion is null");
		ASSERT(pEntitiyHolder->GetEntity() != entt::null, "wrong entity, hitted by explosion");
		ASSERT(registry.has<HealthComponent>(pEntitiyHolder->GetEntity()), "no health component on entity hitted by explosion");

		registry.get<HealthComponent>(pEntitiyHolder->GetEntity()).hp -= pGrenade->GetDamage();
	}
}

void godot::ECSWorld::_register_methods()
{
	register_method((char*)"_init", &ECSWorld::_init);
	register_method((char*)"_ready", &ECSWorld::_ready);
	register_method((char*)"_input", &ECSWorld::HandleInputEvent);
	register_method((char*)"_process", &ECSWorld::_process);
	register_method((char*)"_physics_process", &ECSWorld::_physics_process);
	register_method((char*)"_on_Pickable_picked_up", &ECSWorld::_on_Pickable_picked_up);
	register_method((char*)"_on_Throwable_hit", &ECSWorld::_on_Throwable_hit);
	register_method((char*)"_on_Grenade_explosion", &ECSWorld::_on_Grenade_explosion);
}

void godot::ECSWorld::_init()
{
	utils::InitPhysicLayers();

	//TODO: try to move physics to separate thread
	//TODO: check what systems really should be in phys proc
	//setup physics systems
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new PlayerVelocitySystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new KinematicMovementSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new GravitySystem()));
	//must always follow GravitySystem
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new JumpSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new PlayerRotationSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new LookAtSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new MeleeAttackSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new RangedAttackSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new ThrowAttackSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new PatrolSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new LookAroundSystem()));
	//TODO: should it be in phys proc?
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new NavAgentSystem()));
	m_physics_systems.push_back(std::unique_ptr<BaseSystem>(new PursuingSystem()));
	
	//setup systems
	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new SimpleFollowSystem()));
	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new DestroyDeadSystem()));

	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new WeaponChooseSystem()));
	registry.on_construct<entt::tag<CurrentWeaponMeleeTag> >().connect<&WeaponChooseSystem::OnMeleeTagConstruct>();
	registry.on_construct<entt::tag<CurrentWeaponRangedTag> >().connect<&WeaponChooseSystem::OnRangedTagConstruct>();
	registry.on_construct<entt::tag<CurrentWeaponThrowableTag> >().connect<&WeaponChooseSystem::OnThrowableTagConstruct>();

	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new HealthMonitoringSystem()));
	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new FleeingSystem()));
	m_process_systems.push_back(std::unique_ptr<BaseSystem>(new BillboardRotationSystem()));
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
		registry.clear();
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