#include "PursueStateSystem.h"

#include <Spatial.hpp>
#include <OS.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Components/AIComponents/PatrolComponents.h"
#include "../../Components/AttackComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"

#include "DecisionMakingHelper.h"

void godot::PursueStateSystem::operator()(float delta, entt::registry& registry)
{
	auto players = registry.view<entt::tag<PlayerTag>, Spatial*>();

	auto view = registry.view<entt::tag<BotTag>, PursuingStateComponent, PatrolmanComponent, MeleeAttackComponent, HealthComponent, Spatial*>();
	view.less([this, &registry, &players](entt::entity entity, PursuingStateComponent& pursuingComp
		, PatrolmanComponent patrolComp, MeleeAttackComponent& meleeComp, HealthComponent healthComp, Spatial* pSpatial)
	{
		bool validTarget = registry.valid(pursuingComp.target);
		if (validTarget && CanSeeTarget(players.get<Spatial*>(pursuingComp.target), patrolComp, pSpatial))
			pursuingComp.targetLostMsec = -1;
		else
		{
			int64_t ticksMsec = godot::OS::get_singleton()->get_ticks_msec();
			if (pursuingComp.targetLostMsec < 0)
				pursuingComp.targetLostMsec = ticksMsec;
			else if (pursuingComp.targetLostMsec + utils::SecondsToMillis(patrolComp.looseTargetTime) <= ticksMsec)
				validTarget = false;
		}

		//TODO: move somwhere like DecisionMakingView
		//to flee transition
		const float criticalProportion = 0.5f;
		if (healthComp.ProportionOfMax() <= criticalProportion)
			registry.assign<entt::tag<FleeStateTag> >(entity);
		else if (validTarget)
		{
			//to attack transition
			if (meleeComp.distance >= GetDistanceToTarget(registry, pursuingComp.target, pSpatial))
			{
				registry.assign<entt::tag<MeleeAttackStateTag> >(entity);
				registry.assign<TargetLockComponent>(entity).target = pursuingComp.target;
			}
		}
		//to patrol transition
		else
			registry.assign<entt::tag<PatrolStateTag> >(entity);
	});
}