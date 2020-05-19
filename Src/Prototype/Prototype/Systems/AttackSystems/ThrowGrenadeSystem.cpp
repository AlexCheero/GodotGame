#include "ThrowGrenadeSystem.h"

#include <OS.hpp>
#include <SceneTree.hpp>
#include <Viewport.hpp>
#include <RigidBody.hpp>

#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Nodes/EntityView.h"

#include "../../Utils/Utils.h"

//TODO: almost entirely copied from ThrowAttackSystem. implement different logick
void godot::ThrowGrenadeSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<AttackPressedTag, CurrentWeaponGrenadeTag, GrenadeAttackComponent, BoundsComponent, Spatial*>();
	view.each([&registry](entt::entity entity, GrenadeAttackComponent& attackComp, BoundsComponent bounds, Spatial* pAttackerSpatial)
	{
		if (!utils::Expired(attackComp.attackTime, attackComp.prevHitTime))
			return;

		Node* grenadeNode = attackComp.grenadeScene->instance();
		attackComp.ammoCount--;
		ASSERT(attackComp.ammoCount >= 0, "negative ammo count");
		if (attackComp.ammoCount == 0 /*&& throw on out of ammo*/)
			registry.remove<GrenadeAttackComponent>(entity);

		Godot::print("Throw grenade!");

		pAttackerSpatial->get_tree()->get_current_scene()->add_child(grenadeNode);

		RigidBody* pRB = Object::cast_to<RigidBody>(grenadeNode);
		
		pRB->add_collision_exception_with(pAttackerSpatial);

		Transform grenadeTransform = pRB->get_transform();
		Transform attackerTransform = pAttackerSpatial->get_transform();
		static constexpr float throwMargin = 1.f;
		grenadeTransform.origin = attackerTransform.origin + attackerTransform.basis.z * (bounds.length + throwMargin);
		pRB->set_transform(grenadeTransform);
		pRB->apply_central_impulse(attackerTransform.basis.z * attackComp.force);

		entt::entity grenadeEntity = registry.create();
		registry.emplace<Spatial*>(grenadeEntity, Object::cast_to<Spatial>(grenadeNode));

		EntityView* pGrenadeView = Object::cast_to<EntityView>(grenadeNode->get_node("EntityView"));
		ASSERT(pGrenadeView != nullptr, "grenade entity view is null");

		GrenadeComponent grenComp;
		bool converted = pGrenadeView->ConvertToComponent(grenComp);
		ASSERT(converted, "grenade view have no GrenadeComponent");
		grenComp.startTime = OS::get_singleton()->get_ticks_msec();

		registry.emplace<GrenadeComponent>(grenadeEntity, grenComp);
	});
}