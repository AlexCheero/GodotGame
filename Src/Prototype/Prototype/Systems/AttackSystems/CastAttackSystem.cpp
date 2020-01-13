#include "CastAttackSystem.h"

#include <Spatial.hpp>
#include <OS.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Components/NodeComponents/EnemyNodeComponent.h"

#include "../../Utils.h"

void godot::CastAttackSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<CastAttackComponent, InputComponent, Spatial*>();
	view.each([&registry, this](entt::entity entity, CastAttackComponent& attackComp, InputComponent input, Spatial* pAttackerSpatial)
	{
		if (!CanAttack(input, attackComp.attackTime, attackComp.prevHitTime))
			return;

		Godot::print("Bang!");

		PhysicsDirectSpaceState* spaceState = pAttackerSpatial->get_world()->get_direct_space_state();
		Transform attackerTransform = pAttackerSpatial->get_transform();
		Vector3 from = attackerTransform.origin;
		Vector3 to = from + attackerTransform.basis.z * attackComp.distance;
		Dictionary rayHit = spaceState->intersect_ray(from, to, Array(), utils::GetLayerByName("Enemy"));

		if (rayHit.empty())
			return;

		Object* pObj = Node::___get_from_variant(rayHit["collider"]);
		entt::entity enemyEntity = Object::cast_to<EnemyNodeComponent>(pObj)->GetEntity();

		HealthComponent& enemyHealthComp = registry.get<HealthComponent>(enemyEntity);
		enemyHealthComp.hp -= attackComp.damage;

		if (enemyHealthComp.hp <= 0)
		{
			Godot::print("Cast kill!");
			enemyHealthComp.hp = 0;
			registry.assign<entt::tag<DeadTag> >(enemyEntity);
		}
		else
			Godot::print("Cast hit!");
	});
}