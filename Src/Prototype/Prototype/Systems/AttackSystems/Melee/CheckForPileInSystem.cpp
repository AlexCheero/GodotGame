#include "CheckForPileInSystem.h"

#include "../../../Components/AttackComponents.h"
#include "../../../Components/InputComponents.h"

void godot::CheckForPileInSystem::Tick(float delta, entt::registry& registry)
{
	auto view = registry.view<MeleeAttackEvent, CurrentWeaponMeleeTag, TargetLockComponent, MeleeAttackComponent, Spatial*>(entt::exclude<InAirTag, PileInTag>);
	view.each([&registry](entt::entity entity, TargetLockComponent lockComp, MeleeAttackComponent melee, Spatial* pSpatial)
	{
		ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
		Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
		Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		if (distanceToTarget > melee.GetCurrentHit().maxDistance)
			registry.emplace<PileInTag>(entity);
	});
}