#include "MeleeAttackSystem.h"

#include <World.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <Dictionary.hpp>
#include <Input.hpp>
#include <OS.hpp>

#include "core/math/math_funcs.h"

#include "../../Nodes/EntityHolderNode.h"

#include "../../Components/SimpleComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Utils/Utils.h"

const float INTERSECT_RESULTS_NUM = 16.f;

godot::Array godot::MeleeAttackSystem::GetIntersects(Spatial* pAttackerSpatial, float distance, String layerName)
{
	m_attackShape->set_radius(distance);
	m_params->set_collision_mask(utils::GetLayerByName(layerName));
	m_params->set_shape(m_attackShape);
	Transform attackerTransform = pAttackerSpatial->get_global_transform();
	m_params->set_transform(attackerTransform);

	PhysicsDirectSpaceState* spaceState = pAttackerSpatial->get_world()->get_direct_space_state();
	return spaceState->intersect_shape(m_params, INTERSECT_RESULTS_NUM);
}

godot::Vector3 godot::MeleeAttackSystem::GetDirToTarget(Spatial* pAttackerSpatial, entt::registry& registry, entt::entity lockedTarget)
{
	ASSERT(registry.has<Spatial*>(lockedTarget), "lockedTarget has no spatial");
	Spatial* pTargetSpatial = registry.get<Spatial*>(lockedTarget);

	Transform attackerTransform = pAttackerSpatial->get_global_transform();
	Vector3 enemyPosition = pTargetSpatial->get_global_transform().origin;
	Vector3 dirToTarget = enemyPosition - attackerTransform.origin;
	dirToTarget.normalize();

	return dirToTarget;
}

bool godot::MeleeAttackSystem::ChecktargetEntity(entt::registry& registry, entt::entity lockedTarget)
{
	return lockedTarget != entt::null && registry.valid(lockedTarget) && !registry.has<entt::tag<DeadTag> >(lockedTarget);
}

godot::MeleeAttackSystem::MeleeAttackSystem()
{
	m_params = Ref<PhysicsShapeQueryParameters>(PhysicsShapeQueryParameters::_new());
	m_params->set_collide_with_areas(false);
	m_params->set_collide_with_bodies(true);

	m_attackShape = Ref<SphereShape>(SphereShape::_new());
}

//TODO: implement proper hth with blocks and stuff
//TODO: call such systems only by input callback
void godot::MeleeAttackSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<entt::tag<CurrentWeaponMeleeTag>, MeleeAttackComponent, InputComponent, RotationDirectionComponent, Spatial*>(ExcludeDead);
	view.less([&registry, this](entt::entity entity, MeleeAttackComponent& attackComp, InputComponent input
		, RotationDirectionComponent& rotComp, Spatial* pAttackerSpatial)
	{
		if (ChecktargetEntity(registry, attackComp.lockedTarget))
			rotComp.direction = GetDirToTarget(pAttackerSpatial, registry, attackComp.lockedTarget);
		
		if (!CanAttack(input, attackComp.attackTime, attackComp.prevHitTimeMillis))
			return;
		
		Godot::print("Splash!");

		//TODO: intersect only with something that can be hitted
		Array intersects = GetIntersects(pAttackerSpatial, attackComp.distance, attackComp.collisionLayerName);
		if (intersects.size() == 0)
			return;

		Dictionary dict = intersects[0];//TODO: hits only first intersected, implement area hits
		Object* pHittedObj = Node::___get_from_variant(dict["collider"]);

		if (!pHittedObj)
			return;

		EntityHolderNode* pEntityHolder = Object::cast_to<EntityHolderNode>(pHittedObj);
		entt::entity targetEntity = pEntityHolder->GetEntity();
		ASSERT(pEntityHolder != nullptr, "target pEntityHolder is null");
		ASSERT(targetEntity != entt::null, "target entity is null");
		ASSERT(registry.valid(targetEntity), "invalid target entity");
		//TODO: probably this should be runtime check
		ASSERT(!registry.has<entt::tag<DeadTag> >(targetEntity), "");

		if (attackComp.lockedTarget == targetEntity)
			return;

		attackComp.lockedTarget = targetEntity;

		Vector3 dirToTarget = GetDirToTarget(pAttackerSpatial, registry, attackComp.lockedTarget);;
		float angleCos = dirToTarget.dot(pAttackerSpatial->get_global_transform().basis.z);
		float angle = Math::rad2deg(Math::acos(angleCos));
		if (angle <= attackComp.angle)
			rotComp.direction = dirToTarget;
	});
}