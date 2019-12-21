#include "AttackSystem.h"

#include <World.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <Dictionary.hpp>
#include <Input.hpp>
#include <OS.hpp>

#include "core/math/math_funcs.h"

#include "../Components/Enemy.h"
#include "../Components/AttackComponent.h"
#include "../Components/InputComponents.h"
#include "../Utils.h"

const float INTERSECT_RESULTS_NUM = 16.f;

godot::Array godot::AttackSystem::GetIntersects(Spatial* pAttackerSpatial, float distance)
{
	m_attackShape->set_radius(distance);
	m_params->set_shape(m_attackShape);
	Transform attackerTransform = pAttackerSpatial->get_global_transform();
	m_params->set_transform(attackerTransform);

	PhysicsDirectSpaceState* spaceState = pAttackerSpatial->get_world()->get_direct_space_state();
	return spaceState->intersect_shape(m_params, INTERSECT_RESULTS_NUM);
}

godot::AttackSystem::AttackSystem()
{
	m_params = static_cast< Ref<PhysicsShapeQueryParameters> >(PhysicsShapeQueryParameters::_new());
	m_params->set_collision_mask(utils::GetLayerByName("Enemy"));
	m_params->set_collide_with_areas(false);
	m_params->set_collide_with_bodies(true);

	m_attackShape = static_cast<Ref< SphereShape> >(SphereShape::_new());
}

void godot::AttackSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<AttackComponent, InputComponent, Spatial*>();
	view.each([&registry, this](entt::entity entity, AttackComponent& attackComp, InputComponent input, Spatial* pAttackerSpatial)
	{
		if (!input.attack)
			return;

		int64_t currTime = godot::OS::get_singleton()->get_ticks_msec();
		if (attackComp.prevHitTime + utils::SecondsToMillis(attackComp.attackTime) > currTime)
			return;
		attackComp.prevHitTime = currTime;

		Array intersects = GetIntersects(pAttackerSpatial, attackComp.distance);
		if (intersects.size() == 0)
			return;
		
		Dictionary dict = intersects[0];//TODO: hits only first intersected, implement area hits
		Object* pObj = Node::___get_from_variant(dict["collider"]);
		
		Vector3 enemyPosition = Object::cast_to<Spatial>(pObj)->get_global_transform().origin;
		Transform attackerTransform = pAttackerSpatial->get_global_transform();
		if (!CheckAttackAngle(attackerTransform.origin, -attackerTransform.basis.z, enemyPosition, attackComp.angle))
			return;

		entt::entity enemyEntity = Object::cast_to<Enemy>(pObj)->GetEntity();
		
		HealthComponent& enemyHealthComp = registry.get<HealthComponent>(enemyEntity);
		enemyHealthComp.hp -= attackComp.damage;

		if (enemyHealthComp.hp <= 0)
		{
			Godot::print("Kill!");
			enemyHealthComp.hp = 0;
			registry.assign<entt::tag<DeadTag> >(enemyEntity);
		}
		else
			Godot::print("Hit!");
	});
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
bool godot::AttackSystem::CheckAttackAngle(Vector3 attackerPosition, Vector3 attackerDirection, Vector3 targetPosition, float maxAngle)
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