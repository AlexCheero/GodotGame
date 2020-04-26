#include "HTHStateSystem.h"

#include <OS.hpp>

#include "../../../Components/AIComponents/FSMStateComponents.h"
#include "../../../Components/AttackComponents.h"
#include "../../../Components/InputComponents.h"
#include "../../../Components/AIComponents/NavigationComponents.h"

#include "DecisionMakingHelper.h"

void godot::HTHStateSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<BotTag, MeleeAttackStateTag, InputComponent
		, MeleeAttackComponent, HealthComponent, Spatial*
		, TargetLockComponent>();
	view.less([this, &registry](entt::entity entity, InputComponent& inputComp, MeleeAttackComponent& meleeComp
		, HealthComponent healthComp, Spatial* pSpatial, TargetLockComponent lockComp)
	{
		int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
		bool attackInput = meleeComp.prevHitTimeMillis + utils::SecondsToMillis(meleeComp.attackTime) <= currTimeMillis;
		inputComp.Set(EInput::Attack, attackInput);

		//to flee transition
		if (healthComp.IsHealthCritical())
			registry.assign<FleeStateTag>(entity);
		//to pursuit transition
		else if (registry.valid(lockComp.target) && meleeComp.maxDistance < GetDistanceToTarget(registry, lockComp.target, pSpatial))
			registry.assign<PursuingStateComponent>(entity, lockComp.target);
		//to patrol transition
		else if (!registry.valid(lockComp.target) || registry.has<DeadTag>(lockComp.target))
			registry.assign<PatrolStateTag>(entity);
	});
}