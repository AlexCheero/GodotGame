#include "ThrowAttackSystem.h"

#include <OS.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>

#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Nodes/ThrowableWeaponNode.h"
#include "../../Nodes/EntityView.h"

#include "../../Utils/Utils.h"

void godot::ThrowAttackSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<AttackPressedTag, CurrentWeaponThrowableTag, ThrowableAttackComponent, BoundsComponent, Spatial*>();
	view.each([&registry](entt::entity entity, ThrowableAttackComponent& attackComp, BoundsComponent bounds, Spatial* pAttackerSpatial)
	{
		if (!utils::Expired(attackComp.attackTime, attackComp.prevHitTime))
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
		
		pRB->add_collision_exception_with(pAttackerSpatial);

		Transform throwableTransform = pRB->get_transform();
		Transform attackerTransform = pAttackerSpatial->get_transform();
		static constexpr float throwMargin = 1.f;
		throwableTransform.origin = attackerTransform.origin + attackerTransform.basis.z * (bounds.length + throwMargin);
		pRB->set_transform(throwableTransform);
		pRB->apply_central_impulse(attackerTransform.basis.z * attackComp.force);

		ThrowableWeaponNode* throwable = Object::cast_to<ThrowableWeaponNode>(throwableNode);

		//TODO: split grenades and throwables
		if (throwable)
			throwable->SetThrowForce(attackComp.force);
		else
		{
			entt::entity grenadeEntity = registry.create();
			registry.emplace<Spatial*>(grenadeEntity, Object::cast_to<Spatial>(throwableNode));

			EntityView* pGrenadeView = Object::cast_to<EntityView>(throwableNode->get_node("EntityView"));
			ASSERT(pGrenadeView != nullptr, "grenade entity view is null");

			GrenadeComponent grenComp;
			bool converted = pGrenadeView->ConvertToComponent(grenComp);
			ASSERT(converted, "grenade view have no GrenadeComponent");
			grenComp.startTime = OS::get_singleton()->get_ticks_msec();

			registry.emplace<GrenadeComponent>(grenadeEntity, grenComp);
		}
	});
}