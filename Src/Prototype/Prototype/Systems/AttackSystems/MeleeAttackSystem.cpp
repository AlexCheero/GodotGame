#include "MeleeAttackSystem.h"

#include <World.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <Dictionary.hpp>
#include <Input.hpp>
#include <OS.hpp>

#include "core/math/math_funcs.h"

#include "../../Nodes/EntityHolderNode.h"

#include "../../Components/SimpleComponents.h"
#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Utils/Utils.h"

const float INTERSECT_RESULTS_NUM = 16.f;

godot::Ref<godot::PhysicsShapeQueryParameters> godot::MeleeAttackSystem::m_params;
godot::Ref<godot::SphereShape> godot::MeleeAttackSystem::m_attackShape;

void godot::MeleeAttackSystem::InitParams()
{
	if (m_params.is_valid() && m_attackShape.is_valid())
		return;

	m_params = Ref<PhysicsShapeQueryParameters>(PhysicsShapeQueryParameters::_new());
	m_params->set_collide_with_areas(false);
	m_params->set_collide_with_bodies(true);

	m_attackShape = Ref<SphereShape>(SphereShape::_new());
}

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

//TODO: implement proper hth with blocks and stuff
//TODO: call such systems only by input callback
void godot::MeleeAttackSystem::OnMeleeInputTagConstruct(entt::registry& registry, entt::entity entity)
{
	//TODO: assert m_params.is_valid() && m_attackShape.is_valid()
	//TODO: assert registry.has all this components
	MeleeAttackComponent& attackComp = registry.get<MeleeAttackComponent>(entity);
	InputComponent input = registry.get<InputComponent>(entity);
	Spatial* pAttackerSpatial = registry.get<Spatial*>(entity);

	int64_t currTime = godot::OS::get_singleton()->get_ticks_msec();
	if (attackComp.prevHitTime + utils::SecondsToMillis(attackComp.attackTime) > currTime)
		return;

	attackComp.prevHitTime = currTime;

	//TODO: bot keeps splashing after killing player
	Godot::print("Splash!");

	Array intersects = GetIntersects(pAttackerSpatial, attackComp.distance, attackComp.collisionLayerName);
	if (intersects.size() == 0)
		return;

	Object* pHittedObj = nullptr;
	for (int i = 0; i < intersects.size(); i++)
	{
		Dictionary dict = intersects[i];//TODO: hits only first intersected, implement area hits
		Object* pObj = Node::___get_from_variant(dict["collider"]);

		Object* pCastHitResult = utils::CastFromSpatial(pAttackerSpatial, pAttackerSpatial->get_global_transform().get_basis().z, attackComp.distance);
		if (pObj == pCastHitResult)
			pHittedObj = pCastHitResult;
	}

	if (!pHittedObj)
		return;

	Vector3 enemyPosition = Object::cast_to<Spatial>(pHittedObj)->get_global_transform().origin;
	Transform attackerTransform = pAttackerSpatial->get_global_transform();
	if (!CheckAttackAngle(attackerTransform.origin, attackerTransform.basis.z, enemyPosition, attackComp.angle))
		return;

	entt::entity enemyEntity = Object::cast_to<EntityHolderNode>(pHittedObj)->GetEntity();

	HealthComponent& enemyHealthComp = registry.get<HealthComponent>(enemyEntity);
	enemyHealthComp.hp -= attackComp.damage;

	Godot::print("melee hit");
}

/*
bool godot::AttackSystem::CheckAttackAngle(Vector3 attackerPosition, Vector3 attackerDirection, Vector3 targetPosition, float maxAngle)
{
	Vector3 toEnemyDirection = targetPosition - attackerPosition;
	toEnemyDirection.normalize();
	float angleCos = toEnemyDirection.dot(attackerDirection);
	float angle = Math::rad2deg(Math::acos(angleCos));

	return angle <= maxAngle;
}
*/

//check only flat (in (x,z) plane) angle. without considering y position of target
bool godot::MeleeAttackSystem::CheckAttackAngle(Vector3 attackerPosition, Vector3 attackerDirection, Vector3 targetPosition, float maxAngle)
{
	Vector2 targetFlatPosition = utils::FlatVector(targetPosition);
	Vector2 attackerFlatPosition = utils::FlatVector(attackerPosition);
	Vector2 toEnemyFlatDirection = targetFlatPosition - attackerFlatPosition;
	toEnemyFlatDirection.normalize();
	Vector2 attackerFlatDirection = utils::FlatVector(attackerDirection);
	float angleCos = toEnemyFlatDirection.dot(attackerFlatDirection);
	float angle = Math::rad2deg(Math::acos(angleCos));

	return angle <= maxAngle;
}