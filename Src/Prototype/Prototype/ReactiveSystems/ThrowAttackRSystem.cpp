#include "ThrowAttackRSystem.h"

#include <OS.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>

#include "../Components/AttackComponents.h"
#include "../Components/InputComponents.h"
#include "../Nodes/ThrowableWeaponNode.h"
#include "../Nodes/EntityView.h"

#include "../Utils/Utils.h"

namespace //private
{
	void OnInputPressed(entt::registry& registry, entt::entity entity)
	{
		if (!registry.has<CurrentWeaponThrowableTag, ThrowableAttackComponent>(entity))
			return;

		ASSERT(registry.has<BoundsComponent>(entity), "entity has no VelocityComponent");
		ASSERT(registry.has<godot::Spatial*>(entity), "entity has no Spatial*");

		ThrowableAttackComponent& attackComp = registry.get<ThrowableAttackComponent>(entity);
		BoundsComponent bounds = registry.get<BoundsComponent>(entity);
		godot::Spatial* pAttackerSpatial = registry.get<godot::Spatial*>(entity);

		if (!utils::Expired(attackComp.attackTime, attackComp.prevHitTime))
			return;

		godot::Node* throwableNode = attackComp.throwableScene->instance();
		attackComp.ammoCount--;
		ASSERT(attackComp.ammoCount >= 0, "negative ammo count");
		//TODO: instantly melee hits or don't changes weapon at all, after throwing weapon. fix this after refactoring whole input system
		//		input system already refactored
		if (attackComp.ammoCount == 0 /*&& throw on out of ammo*/)
			registry.remove<ThrowableAttackComponent>(entity);

		godot::Godot::print("Throw!");

		pAttackerSpatial->get_tree()->get_current_scene()->add_child(throwableNode);

		godot::RigidBody* pRB = godot::Object::cast_to<godot::RigidBody>(throwableNode);

		godot::Transform throwableTransform = pRB->get_transform();
		godot::Transform attackerTransform = pAttackerSpatial->get_transform();
		//TODO: hits thrower. take throwable dimension into account
		throwableTransform.origin = attackerTransform.origin + attackerTransform.basis.z * bounds.length;
		pRB->set_transform(throwableTransform);
		pRB->apply_central_impulse(attackerTransform.basis.z * attackComp.force);

		godot::ThrowableWeaponNode* throwable = godot::Object::cast_to<godot::ThrowableWeaponNode>(throwableNode);

		//TODO: split grenades and throwables
		if (throwable)
			throwable->SetThrowForce(attackComp.force);
		else
		{
			entt::entity grenadeEntity = registry.create();
			registry.assign<godot::Spatial*>(grenadeEntity, godot::Object::cast_to<godot::Spatial>(throwableNode));

			godot::EntityView* pGrenadeView = godot::Object::cast_to<godot::EntityView>(throwableNode->get_node("EntityView"));
			ASSERT(pGrenadeView != nullptr, "grenade entity view is null");

			GrenadeComponent grenComp;
			bool converted = pGrenadeView->ConvertToComponent(grenComp);
			ASSERT(converted, "grenade view have no GrenadeComponent");
			grenComp.startTime = godot::OS::get_singleton()->get_ticks_msec();

			registry.assign<GrenadeComponent>(grenadeEntity, grenComp);
		}
	}
}

void godot::ThrowAttackRSystem::Init(entt::registry& registry)
{
	registry.on_construct<AttackPressedTag>().connect<&OnInputPressed>();
}
