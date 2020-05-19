#include "MeleeStateSystem.h"

#include <OS.hpp>

#include "../../../Components/AIComponents/FSMStateComponents.h"
#include "../../../Components/AttackComponents.h"
#include "../../../Components/InputComponents.h"
#include "../../../Components/AIComponents/NavigationComponents.h"

#include "DecisionMakingHelper.h"

void godot::MeleeStateSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<BotTag, MeleeAttackStateTag
		, MeleeAttackComponent, PrevAttackTime, HealthComponent, Spatial*
		, TargetLockComponent, DecisionMakingComponent>(entt::exclude<AttackPressedTag>);
	view.less([this, &registry](entt::entity entity, MeleeAttackComponent meleeComp, PrevAttackTime prevAttack
		, HealthComponent healthComp, Spatial* pSpatial, TargetLockComponent lockComp, DecisionMakingComponent decisionComp)
	{
		int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
		bool attackInput = prevAttack.millis + utils::SecondsToMillis(meleeComp.GetCurrentHit().attackTime) <= currTimeMillis;
		//TODO: check if this could keep input event on state transition
		if (attackInput)
			registry.assign<AttackPressedTag>(entity);

		//TODO: make better melee system. to flee transition commented for now
		//to flee transition
		//if (healthComp.hp <= decisionComp.criticalHp)
		//	registry.assign<FleeStateTag>(entity);
		//to pursuit transition
		else if (registry.valid(lockComp.target) && meleeComp.GetCurrentHit().maxDistance < GetDistanceToTarget(registry, lockComp.target, pSpatial))
			registry.assign<PursuingStateComponent>(entity, lockComp.target);
		//to patrol transition
		else if (!registry.valid(lockComp.target) || registry.has<DeadTag>(lockComp.target))
			registry.assign<PatrolStateTag>(entity);
	});
}