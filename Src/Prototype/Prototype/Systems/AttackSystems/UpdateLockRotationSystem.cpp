#include "UpdateLockRotationSystem.h"

#include "../../Components/AttackComponents.h"

bool godot::UpdateLockRotationSystem::ChecktargetEntity(entt::registry& registry, entt::entity lockedTarget)
{
	return registry.valid(lockedTarget) && !registry.has<DeadTag>(lockedTarget);
}

void godot::UpdateLockRotationSystem::Tick(float delta, entt::registry& registry)
{
	//TODO_melee: make unlock logick
	auto lockedView = registry.view<CurrentWeaponMeleeTag, TargetLockComponent, RotationDirectionComponent, Spatial*>();
	lockedView.each([&registry](entt::entity entity, TargetLockComponent lockComp, RotationDirectionComponent& rotComp, Spatial* pSpatial)
	{
		if (ChecktargetEntity(registry, lockComp.target))
		{
			ASSERT(registry.has<Spatial*>(lockComp.target), "target has no spatial");
			Spatial* pTargetSpatial = registry.get<Spatial*>(lockComp.target);
			Vector3 dirToTarget = pTargetSpatial->get_global_transform().get_origin() - pSpatial->get_global_transform().get_origin();
			rotComp.direction = dirToTarget.normalized();
		}
		else
			registry.remove<TargetLockComponent>(entity);
	});
}