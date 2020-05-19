#include "MeleeLockTargetSystem.h"

#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "../../../Components/AttackComponents.h"
#include "../../../Components/InputComponents.h"

#include "../../../Nodes/EntityView.h"

godot::Array godot::MeleeLockTargetSystem::GetIntersects(Spatial* pAttackerSpatial, float distance, String layerName)
{
	Array exclude;
	exclude.push_back(pAttackerSpatial);
	params->set_exclude(exclude);

	attackShape->set_radius(distance);
	params->set_collision_mask(utils::GetLayerByName(layerName));
	params->set_shape(attackShape);
	Transform attackerTransform = pAttackerSpatial->get_global_transform();
	params->set_transform(attackerTransform);

	PhysicsDirectSpaceState* spaceState = pAttackerSpatial->get_world()->get_direct_space_state();
	return spaceState->intersect_shape(params, INTERSECT_RESULTS_NUM);
}

godot::MeleeLockTargetSystem::MeleeLockTargetSystem()
{
	params = Ref<PhysicsShapeQueryParameters>(PhysicsShapeQueryParameters::_new());
	params->set_collide_with_areas(false);
	params->set_collide_with_bodies(true);

	attackShape = Ref<SphereShape>(SphereShape::_new());
}

void godot::MeleeLockTargetSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<AttackPressedTag, CurrentWeaponMeleeTag, MeleeAttackComponent, Spatial*>(entt::exclude<TargetLockComponent>);
	view.less([this, &registry](entt::entity entity, MeleeAttackComponent attackComp, Spatial* pSpatial)
	{
		//TODO_melee: implement target change when already have locked target
		//TODO_melee: do not lock on ally even if friendly fire is on
		Array intersects = GetIntersects(pSpatial, attackComp.GetCurrentHit().maxDistance, "Character");
		if (intersects.size() == 0)
			return;

		Dictionary dict = intersects[0];//TODO_melee: hits only first intersected, choose between multiple enemies
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

		ASSERT(targetEntity != entity, "overlapped self");
		ASSERT(targetEntity != entt::null, "target entity is null");
		ASSERT(registry.valid(targetEntity), "invalid target entity");
		ASSERT(!registry.has<DeadTag>(targetEntity), "entity is already dead!");

		registry.assign<TargetLockComponent>(entity).target = targetEntity;
	});
}