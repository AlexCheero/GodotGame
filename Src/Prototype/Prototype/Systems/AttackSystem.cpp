#include "AttackSystem.h"

#include <Spatial.hpp>

#include "../Components/Enemy.h"

void godot::AttackSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();
	if (!pInput->is_action_just_pressed("attack"))
		return;

	registry.view<AttackComponent, Spatial*>().each([&registry](AttackComponent attackComp, Spatial* pAttackerSpatial)
	{
		Vector3 attackPos = pAttackerSpatial->get_global_transform().origin;
		registry.view<Enemy*, HealthComponent, Spatial*>().each(
		[&registry, attackPos, attackComp](entt::entity enemyEntity, Enemy* pEnemy, HealthComponent& enemyHealthComp, Spatial* pEnemySpatial)
		{
			if (enemyHealthComp.hp <= 0)
				return;

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
}
