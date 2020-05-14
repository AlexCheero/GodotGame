#include "CheckForPileInRSystem.h"

#include "../Components/AttackComponents.h"
#include "../Components/InputComponents.h"

namespace //private
{
	void OnInputPressed(entt::registry& registry, entt::entity entity)
	{
		//TODO0: make revision of all reactive includes/excludes!!!
		if (!registry.has<TargetLockComponent, CurrentWeaponMeleeTag>(entity) || registry.has<InAirTag>(entity) || registry.has<PileInTag>(entity))
			return;

		ASSERT(registry.has<MeleeAttackComponent>(entity), "entity has no MeleeAttackComponent");
		ASSERT(registry.has<godot::Spatial*>(entity), "entity has no Spatial*");

		TargetLockComponent lockComp = registry.get<TargetLockComponent>(entity);
		MeleeAttackComponent melee = registry.get<MeleeAttackComponent>(entity);
		godot::Spatial* pSpatial = registry.get<godot::Spatial*>(entity);

		ASSERT(registry.has<godot::Spatial*>(lockComp.target), "target has no spatial");
		godot::Spatial* pTargetSpatial = registry.get<godot::Spatial*>(lockComp.target);
		godot::Vector3 toTargetDirection = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
		float distanceToTarget = toTargetDirection.length();
		if (distanceToTarget > melee.GetCurrentHit().maxDistance)
			registry.assign<PileInTag>(entity);
	}
}

void godot::CheckForPileInRSystem::Init(entt::registry& registry)
{
	registry.on_construct<AttackPressedTag>().connect<&OnInputPressed>();
}