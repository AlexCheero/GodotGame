#include "HTHStateSystem.h"

#include <OS.hpp>

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Components/AttackComponents.h"
#include "../../Components/InputComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"

#include "DecisionMakingHelper.h"

void godot::HTHStateSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<entt::tag<BotTag>, entt::tag<MeleeAttackStateTag>, InputComponent
		, MeleeAttackComponent, HealthComponent, Spatial*
		, TargetLockComponent>();
	view.less([this, &registry](entt::entity entity, InputComponent& inputComp, MeleeAttackComponent& meleeComp
		, HealthComponent healthComp, Spatial* pSpatial, TargetLockComponent lockComp)
	{
		int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
		bool attackInput = meleeComp.prevHitTimeMillis + utils::SecondsToMillis(meleeComp.attackTime) <= currTimeMillis;
		inputComp.Set(EInput::Attack, attackInput);

		//TODO: move somwhere like DecisionMakingView here and from pursueView
		//to flee transition
		const float criticalProportion = 0.5f;
		if (healthComp.ProportionOfMax() <= criticalProportion)
			registry.assign<entt::tag<FleeStateTag> >(entity);
		//to pursuit transition
		else if (registry.valid(lockComp.target) && meleeComp.distance < GetDistanceToTarget(registry, lockComp.target, pSpatial))
			registry.assign<PursuingStateComponent>(entity, lockComp.target);
		//to patrol transition
		else if (!registry.valid(lockComp.target) || registry.has<entt::tag<DeadTag> >(lockComp.target))
			registry.assign<entt::tag<PatrolStateTag> >(entity);
	});
}