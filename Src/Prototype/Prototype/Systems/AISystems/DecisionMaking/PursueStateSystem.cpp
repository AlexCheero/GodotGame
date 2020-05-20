#include "PursueStateSystem.h"

#include <Spatial.hpp>
#include <OS.hpp>

#include "../../../Components/SimpleComponents.h"
#include "../../../Components/AIComponents/FSMStateComponents.h"
#include "../../../Components/AIComponents/PatrolComponents.h"
#include "../../../Components/AttackComponents.h"
#include "../../../Components/AIComponents/NavigationComponents.h"

#include "DecisionMakingHelper.h"

void godot::PursueStateSystem::Tick(float delta, entt::registry& registry)
{
	auto players = registry.view<PlayerTag, Spatial*>();

	auto view = registry.view<BotTag, PursuingStateComponent, PatrolmanComponent, MeleeAttackComponent, HealthComponent, Spatial*, DecisionMakingComponent>();
	view.each([&registry, &players](entt::entity entity, PursuingStateComponent& pursuingComp
		, PatrolmanComponent patrolComp, MeleeAttackComponent meleeComp, HealthComponent healthComp, Spatial* pSpatial, DecisionMakingComponent decisionComp)
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

		//to flee transition
		if (healthComp.hp <= decisionComp.criticalHp)
			registry.emplace<FleeStateTag>(entity);
		else if (validTarget)
		{
			//to attack transition
			if (meleeComp.maxPileInDistance >= GetDistanceToTarget(registry, pursuingComp.target, pSpatial))
			{
				registry.emplace<MeleeAttackStateTag>(entity);
				registry.emplace<TargetLockComponent>(entity).target = pursuingComp.target;
			}
		}
		//to patrol transition
		else
			registry.emplace<PatrolStateTag>(entity);
	});
}