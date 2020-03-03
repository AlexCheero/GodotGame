#pragma once

#include "../BaseSystem.h"

#include <Spatial.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/PatrolComponents.h"

namespace godot
{
	class DecisionMakingFSMSystem : public BaseSystem
	{
	private:
		bool CanSeeTarget(Spatial* pTargetSpatial, PatrolmanComponent patrolman, Spatial* pPatrolSpatial);
		float GetDistanceToTarget(entt::registry& registry, entt::entity target, Spatial* pBotSpatial);
		void OnHitNoticing(entt::registry& registry);
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
