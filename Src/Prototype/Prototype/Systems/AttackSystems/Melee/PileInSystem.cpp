#include "PileInSystem.h"

#include "../../../Components/AttackComponents.h"
#include "../../../Components/InputComponents.h"

void godot::PileInSystem::OnPileInTagDestroyed(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<VelocityComponent>(entity), "entity has no VelocityComponent");
}

godot::PileInSystem::PileInSystem(entt::registry& registry)
{
	//TODO: also cancel if target is dead or when hit is ended, not the whole animation
	registry.on_destroy<AttackAnimPlayingComponent>().connect<&entt::registry::remove_if_exists<PileInTag>>();
	registry.on_destroy<PileInTag>().connect<&PileInSystem::OnPileInTagDestroyed>(this);
}

void godot::PileInSystem::operator()(float delta, entt::registry& registry)
{
	auto pileInView = registry.view<PileInTag, TargetLockComponent, VelocityComponent, SpeedComponent,
									MeleeAttackComponent, Spatial*>();
	pileInView.less([&registry](entt::entity entity, TargetLockComponent lockComp,
												   VelocityComponent& velComp, SpeedComponent speedComp, MeleeAttackComponent melee, Spatial* pSpatial)
	{
		ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
		Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
		Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		
		//TODO: probably remove PileInTag in this case. upd: bad idea to remove. whole pile in system needs refactoring
		//      and minDistance should be removed or at least renamed after that
		if (distanceToTarget <= melee.GetCurrentHit().minDistance)
			return;

		//TODO: probably use input axis instead whenever it possible. for pile in and bot ai systems
		Vector3 toTargetVelocity = toTargetDirection;
		toTargetVelocity.y = 0;
		toTargetVelocity.normalize();
		toTargetVelocity *= speedComp.speed + speedComp.dashSpeed;
		toTargetVelocity.y = velComp.velocity.y;

		velComp.velocity = toTargetVelocity;
	});

	auto cancelPilInView = registry.view<PileInTag, MoveDirInputComponent>();
	cancelPilInView.less([&registry](entt::entity entity, MoveDirInputComponent inputComp)
	{
		if (inputComp.dir.length_squared() != 0)
			registry.remove<PileInTag>(entity);
	});
}