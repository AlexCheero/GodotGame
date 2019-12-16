#include "AttackSystem.h"

#include <Spatial.hpp>
#include <World.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <Dictionary.hpp>
#include <SphereShape.hpp>

#include "core/math/math_funcs.h"

#include "../Components/Enemy.h"

const float INTERSECT_RESULTS_NUM = 16.f;

godot::AttackSystem::AttackSystem()
{
	m_params = (Ref<PhysicsShapeQueryParameters>)PhysicsShapeQueryParameters::_new();
	m_params->set_collision_mask(1 << 2);//Enemy layer TODO: make util to get mask/layer by name
	m_params->set_collide_with_areas(false);
	m_params->set_collide_with_bodies(true);
}

void godot::AttackSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();
	if (!pInput->is_action_just_pressed("attack"))
		return;

	registry.view<AttackComponent, Spatial*>().each([&registry, this](AttackComponent attackComp, Spatial* pAttackerSpatial)
	{
		PhysicsDirectSpaceState* spaceState = pAttackerSpatial->get_world()->get_direct_space_state();
		Ref<SphereShape> shape = SphereShape::_new();
		shape->set_radius(attackComp.distance);
		m_params->set_shape(shape);
		Transform attackerTransform = pAttackerSpatial->get_global_transform();
		m_params->set_transform(attackerTransform);
		Array intersects = spaceState->intersect_shape(m_params, INTERSECT_RESULTS_NUM);

		if (intersects.size() == 0)
			return;
		
		Dictionary dict = intersects[0];//TODO: hits only first intersected, implement area hits
		Object* pObj = Node::___get_from_variant(dict["collider"]);
		
		Vector3 enemyPosition = Object::cast_to<Spatial>(pObj)->get_global_transform().origin;
		if (!CheckAttackAngle(attackerTransform, enemyPosition, attackComp.angle))
			return;

		Enemy* pEnemy = Object::cast_to<Enemy>(pObj);
		entt::entity enemyEntity = pEnemy->GetEntity();
		
		HealthComponent& enemyHealthComp = registry.get<HealthComponent>(enemyEntity);
		enemyHealthComp.hp -= attackComp.damage;

		if (enemyHealthComp.hp <= 0)
		{
			Godot::print("Killed!");
			enemyHealthComp.hp = 0;
			registry.assign<DeadComponent>(enemyEntity);
		}
		else
			Godot::print("Hit!");
	});
}

bool godot::AttackSystem::CheckAttackAngle(Transform attackerTransform, Vector3 enemyPosition, float maxAngle)
{
	Vector3 toEnemyDirection = enemyPosition - attackerTransform.origin;
	toEnemyDirection.normalize();
	float angleCos = toEnemyDirection.dot(-attackerTransform.basis.z);
	float angle = Math::rad2deg(Math::acos(angleCos));

	return angle <= maxAngle;
}
