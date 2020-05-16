#include "CheckForPileInRSystem.h"

#include "../Components/AttackComponents.h"
#include "../Components/InputComponents.h"

void godot::CheckForPileInSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<AttackPressedTag, CurrentWeaponMeleeTag, TargetLockComponent, MeleeAttackComponent, Spatial*>(entt::exclude<InAirTag, PileInTag>);
	view.less([&registry](entt::entity entity, TargetLockComponent lockComp, MeleeAttackComponent melee, Spatial* pSpatial)
	{
		ASSERT(registry.has<godot::Spatial*>(lockComp.target), "target has no spatial");
		godot::Spatial* pTargetSpatial = registry.get<godot::Spatial*>(lockComp.target);
		godot::Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		if (distanceToTarget > melee.GetCurrentHit().maxDistance)
			registry.assign<PileInTag>(entity);
	});
}