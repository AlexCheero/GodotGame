#pragma once

#include "../../BaseSystem.h"

#include <Spatial.hpp>

#include "PatrolStateSystem.h"
#include "PursueStateSystem.h"
#include "MeleeStateSystem.h"
#include "FleeStateSystem.h"

namespace godot
{
	class DecisionMakingFSMSystem : public BaseSystem
	{
	private:
		PatrolStateSystem patrolSystem;
		PursueStateSystem pursueSystem;
		MeleeStateSystem meleeSystem;
		FleeStateSystem fleeSystem;

		void OnTransitionToPatrol(entt::registry& registry, entt::entity entity);
		void OnTransitionToPursuing(entt::registry& registry, entt::entity entity);
		void OnTransitionToFlee(entt::registry& registry, entt::entity entity);
		void OnTransitionToMelee(entt::registry& registry, entt::entity entity);
		void OnHitNoticing(entt::registry& registry, entt::entity entity);
	public:
		DecisionMakingFSMSystem(entt::registry& registry);

		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
