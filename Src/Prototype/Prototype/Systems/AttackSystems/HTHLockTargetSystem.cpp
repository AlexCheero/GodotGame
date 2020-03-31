#include "HTHLockTargetSystem.h"

#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "core/math/math_funcs.h"

#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"

#include "../../Components/Views/EntityView.h"

const float INTERSECT_RESULTS_NUM = 16.f;

godot::Array godot::HTHLockTargetSystem::GetIntersects(Spatial* pAttackerSpatial, float distance, String layerName)
{
	m_attackShape->set_radius(distance);
	m_params->set_collision_mask(utils::GetLayerByName(layerName));
	m_params->set_shape(m_attackShape);
	Transform attackerTransform = pAttackerSpatial->get_global_transform();
	m_params->set_transform(attackerTransform);

	PhysicsDirectSpaceState* spaceState = pAttackerSpatial->get_world()->get_direct_space_state();
	return spaceState->intersect_shape(m_params, INTERSECT_RESULTS_NUM);
}

bool godot::HTHLockTargetSystem::ChecktargetEntity(entt::registry& registry, entt::entity lockedTarget)
{
	return registry.valid(lockedTarget) && !registry.has<entt::tag<DeadTag> >(lockedTarget);
}

godot::HTHLockTargetSystem::HTHLockTargetSystem()
{
	m_params = Ref<PhysicsShapeQueryParameters>(PhysicsShapeQueryParameters::_new());
	m_params->set_collide_with_areas(false);
	m_params->set_collide_with_bodies(true);

	m_attackShape = Ref<SphereShape>(SphereShape::_new());
}

void godot::HTHLockTargetSystem::operator()(float delta, entt::registry& registry)
{
	//TODO: implement target change when already have locked target
	auto withoutTargetView = registry.view<entt::tag<AttackActionTag>, entt::tag<CurrentWeaponMeleeTag>, InputComponent, MeleeAttackComponent, Spatial*>(entt::exclude<TargetLockComponent>);
	withoutTargetView.less([this, &registry](entt::entity entity, InputComponent input, MeleeAttackComponent attackComp, Spatial* pSpatial)
	{
		//TODO: same as for pile in- use different distance for each hit
		Array intersects = GetIntersects(pSpatial, attackComp.distance, attackComp.collisionLayerName);
		if (intersects.size() == 0)
			return;

		Dictionary dict = intersects[0];//TODO: hits only first intersected, choose between multiple enemies
		Object* pHittedObj = Node::___get_from_variant(dict["collider"]);

		if (!pHittedObj)
			return;

		Node* pHittedNode = Object::cast_to<Node>(pHittedObj);
		if (!pHittedNode->has_node("EntityView"))
			return;

		EntityView* pHittedEntityView = Object::cast_to<EntityView>(pHittedNode->get_node("EntityView"));
		if (!pHittedEntityView)
			return;

		entt::entity targetEntity = pHittedEntityView->GetEntity();
		ASSERT(targetEntity != entt::null, "target entity is null");
		ASSERT(registry.valid(targetEntity), "invalid target entity");
		//TODO: probably this should be runtime check
		ASSERT(!registry.has<entt::tag<DeadTag> >(targetEntity), "");

		registry.assign<TargetLockComponent>(entity).target = targetEntity;
	});

	auto lockedView = registry.view<entt::tag<CurrentWeaponMeleeTag>, TargetLockComponent, RotationDirectionComponent, Spatial*>();
	lockedView.less([this, &registry](entt::entity entity, TargetLockComponent lockComp, RotationDirectionComponent& rotComp, Spatial* pSpatial)
	{
		if (ChecktargetEntity(registry, lockComp.target))
		{
			ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
			Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
			Vector3 dirToTarget = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
			rotComp.direction = dirToTarget.normalized();
		}
		else
			registry.remove<TargetLockComponent>(entity);
	});
}