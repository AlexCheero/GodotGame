#include "HealthMonitoringSystem.h"

#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/FSMStateComponents.h"
//TODO: with smarter fleeing system it doesn't needed here
#include "../../Components/AIComponents/PatrolComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"
//--------------------------------------------------------

void godot::HealthMonitoringSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<entt::tag<BotTag>, HealthComponent>(entt::exclude<entt::tag<FleeingTag> >);
	view.less([&registry](entt::entity entity, HealthComponent health)
	{
		//TODO: move somwhere like DecisionMakingView
		const float criticalProportion = 0.5f;
		if (health.ProportionOfMax() > criticalProportion)
			return;

		registry.assign<entt::tag<FleeingTag> >(entity);
		//TODO: make more smart fleeing system and don't remove or even reset it here
		registry.remove<PursuingComponent>(entity);
		registry.remove<PatrolmanComponent>(entity);
		registry.remove<NavPathComponent>(entity);
		//---------------------------------------------------------------------------
	});
}