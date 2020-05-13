#include "HTHLockTargetSystem.h"

#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>
#include <PhysicsShapeQueryParameters.hpp>
#include <Spatial.hpp>
#include <SphereShape.hpp>

//TODO: remove usages of this header
#include "core/math/math_funcs.h"

#include "../Components/AttackComponents.h"
#include "../Components/InputComponents.h"

#include "../Nodes/EntityView.h"

namespace //private
{
	const float INTERSECT_RESULTS_NUM = 16.f;

	godot::Ref<godot::PhysicsShapeQueryParameters> m_params;
	godot::Ref<godot::SphereShape> m_attackShape;

	godot::Array GetIntersects(godot::Spatial* pAttackerSpatial, float distance, godot::String layerName)
	{
		godot::Array exclude;
		exclude.push_back(pAttackerSpatial);
		m_params->set_exclude(exclude);

		m_attackShape->set_radius(distance);
		m_params->set_collision_mask(utils::GetLayerByName(layerName));
		m_params->set_shape(m_attackShape);
		godot::Transform attackerTransform = pAttackerSpatial->get_global_transform();
		m_params->set_transform(attackerTransform);

		godot::PhysicsDirectSpaceState* spaceState = pAttackerSpatial->get_world()->get_direct_space_state();
		return spaceState->intersect_shape(m_params, INTERSECT_RESULTS_NUM);
	}

	void OnInputPressed(entt::registry& registry, entt::entity entity)
	{
		if (!registry.has<CurrentWeaponMeleeTag>(entity) || registry.has<TargetLockComponent>(entity))
			return;

		//TODO: implement target change when already have locked target
		ASSERT(registry.has<MeleeAttackComponent>(entity), "registry has no MeleeAttackComponent");
		ASSERT(registry.has<godot::Spatial*>(entity), "registry has no Spatial*");

		godot::Spatial* pSpatial = registry.get<godot::Spatial*>(entity);
		MeleeAttackComponent attackComp = registry.get<MeleeAttackComponent>(entity);

		//TODO: do not lock on ally even if friendly fire is on
		godot::Array intersects = GetIntersects(pSpatial, attackComp.GetCurrentHit().maxDistance, "Character");
		if (intersects.size() == 0)
			return;

		godot::Dictionary dict = intersects[0];//TODO: hits only first intersected, choose between multiple enemies
		godot::Object* pHittedObj = godot::Node::___get_from_variant(dict["collider"]);

		if (!pHittedObj)
			return;

		godot::Node* pHittedNode = godot::Object::cast_to<godot::Node>(pHittedObj);
		if (!pHittedNode->has_node("EntityView"))
			return;

		godot::EntityView* pHittedEntityView = godot::Object::cast_to<godot::EntityView>(pHittedNode->get_node("EntityView"));
		if (!pHittedEntityView)
			return;

		entt::entity targetEntity = pHittedEntityView->GetEntity();

		ASSERT(targetEntity != entity, "overlapped self");
		ASSERT(targetEntity != entt::null, "target entity is null");
		ASSERT(registry.valid(targetEntity), "invalid target entity");
		ASSERT(!registry.has<DeadTag>(targetEntity), "entity is already dead!");

		registry.assign<TargetLockComponent>(entity).target = targetEntity;
	}
}

void godot::HTHLockTargetRSystem::Init(entt::registry& registry)
{
	m_params = Ref<PhysicsShapeQueryParameters>(PhysicsShapeQueryParameters::_new());
	m_params->set_collide_with_areas(false);
	m_params->set_collide_with_bodies(true);

	m_attackShape = Ref<SphereShape>(SphereShape::_new());

	registry.on_construct<AttackPressedTag>().connect<&OnInputPressed>();
}