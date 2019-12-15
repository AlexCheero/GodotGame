#include "AttackSystem.h"

#include <Spatial.hpp>
#include <World.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <Dictionary.hpp>
#include <PhysicsShapeQueryParameters.hpp>
#include <SphereShape.hpp>

#include "../Components/Enemy.h"

void godot::AttackSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();
	if (!pInput->is_action_just_pressed("attack"))
		return;

	registry.view<AttackComponent, Spatial*>().each([&registry](AttackComponent attackComp, Spatial* pAttackerSpatial)
	{
		PhysicsDirectSpaceState* spaceState = pAttackerSpatial->get_world()->get_direct_space_state();
		Ref<PhysicsShapeQueryParameters> params = PhysicsShapeQueryParameters::_new();//TODO: cache
		Ref<SphereShape> shape = SphereShape::_new();
		shape->set_radius(attackComp.distance);
		params->set_shape(shape);
		params->set_collision_mask(1 << 2);//Enemy layer TODO: make util to get mask/layer by name
		params->set_collide_with_areas(false);
		params->set_collide_with_bodies(true);
		Transform transform = pAttackerSpatial->get_global_transform();
		params->set_transform(transform);
		Array intersects = spaceState->intersect_shape(params, 16);//TODO: hardcode

		if (intersects.size() == 0)
			return;
		
		Dictionary dict = intersects[0];//TODO: hits only first intersected, implement area hits
		Object* pObj = Node::___get_from_variant(dict["collider"]);
		
		//TODO: check attack angle
		Spatial* pEnemySpatial = Object::cast_to<Spatial>(pObj);
		
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
	});

	/*
	registry.view<AttackComponent, Spatial*>().each([&registry](AttackComponent attackComp, Spatial* pAttackerSpatial)
	{
		Vector3 attackPos = pAttackerSpatial->get_global_transform().origin;
		registry.view<Enemy*, HealthComponent, Spatial*>().each(
		[&registry, attackPos, attackComp](entt::entity enemyEntity, Enemy* pEnemy, HealthComponent& enemyHealthComp, Spatial* pEnemySpatial)
		{
			if (enemyHealthComp.hp <= 0)
				return;

			//TODO: calcdistance to surface of the enemy, not to the center
			float distanceToEnemy = (attackPos - pEnemySpatial->get_global_transform().origin).length();

			if (distanceToEnemy <= attackComp.distance)
			{
				Godot::print("Ouch!");
				enemyHealthComp.hp -= attackComp.damage;
				if (enemyHealthComp.hp <= 0)
				{
					Godot::print("Killed!");
					enemyHealthComp.hp = 0;
					registry.assign<DeadComponent>(enemyEntity);
				}
			}
		});
	});
	*/
}
