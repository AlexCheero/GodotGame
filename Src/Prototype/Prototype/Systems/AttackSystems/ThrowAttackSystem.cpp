#include "ThrowAttackSystem.h"

#include <OS.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>

#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Nodes/ThrowableWeaponNode.h"
#include "../../Components/Views/EntityView.h"

void godot::ThrowAttackSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<entt::tag<CurrentWeaponThrowableTag>, ThrowableAttackComponent, InputComponent, Spatial*>();
	view.less([&registry, this](entt::entity entity, ThrowableAttackComponent& attackComp, InputComponent input, Spatial* pAttackerSpatial)
	{
		if (!CanAttack(input, attackComp.attackTime, attackComp.prevHitTime))
			return;

		Node* throwableNode = attackComp.throwableScene->instance();
		attackComp.ammoCount--;
		ASSERT(attackComp.ammoCount >= 0, "negative ammo count");
		//TODO: instantly melee hits or don't changes weapon at all, after throwing weapon. fix this after refactoring whole input system
		//		input system already refactored
		if (attackComp.ammoCount == 0 /*&& throw on out of ammo*/)
			registry.remove<ThrowableAttackComponent>(entity);

		Godot::print("Throw!");

		pAttackerSpatial->get_tree()->get_current_scene()->add_child(throwableNode);

		RigidBody* pRB = Object::cast_to<RigidBody>(throwableNode);

		Transform throwableTransform = pRB->get_transform();
		Transform attackerTransform = pAttackerSpatial->get_transform();
		throwableTransform.origin = attackerTransform.origin;
		throwableTransform.origin += attackerTransform.basis.z;
		pRB->set_transform(throwableTransform);
		pRB->apply_central_impulse(attackerTransform.basis.z * attackComp.force);

		ThrowableWeaponNode* throwable = Object::cast_to<ThrowableWeaponNode>(throwableNode);
		
		//TODO: split grenades and throwables
		if (throwable)
			throwable->SetThrowForce(attackComp.force);
		else
		{
			entt::entity grenadeEntity = registry.create();
			registry.assign<Spatial*>(grenadeEntity, Object::cast_to<Spatial>(throwableNode));
			
			EntityView* pGrenadeView = Object::cast_to<EntityView>(throwableNode->get_node("EntityView"));
			ASSERT(pGrenadeView != nullptr, "grenade entity view is null");

			GrenadeComponent grenComp;
			bool converted = pGrenadeView->ConvertToComponent(grenComp);
			ASSERT(converted, "grenade view have no GrenadeComponent");
			grenComp.startTime = godot::OS::get_singleton()->get_ticks_msec();

			registry.assign<GrenadeComponent>(grenadeEntity, grenComp);
		}
	});
}