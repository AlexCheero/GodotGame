#pragma once

#include "../BaseSystem.h"

#include <Spatial.hpp>

#include "PatrolStateSystem.h"
#include "PursueStateSystem.h"
#include "HTHStateSystem.h"
#include "FleeStateSystem.h"

namespace godot
{
	//TODO: move all decision making code (base system, state systems, helpers etc) to separate filter/folder
	class DecisionMakingFSMSystem : public BaseSystem
	{
	private:
		PatrolStateSystem patrolSystem;
		PursueStateSystem pursueSystem;
		HTHStateSystem hthSystem;
		FleeStateSystem fleeSystem;

		void OnTransitionToPatrol(entt::registry& registry, entt::entity entity);
		void OnTransitionToPursuing(entt::registry& registry, entt::entity entity);
		void OnTransitionToFlee(entt::registry& registry, entt::entity entity);
		void OnTransitionToHTH(entt::registry& registry, entt::entity entity);

		entt::observer hittedOnPatrolObserver;
		entt::observer hittedOutOfPatrolObserver;
		void OnHitNoticing(entt::registry& registry);
	public:
		DecisionMakingFSMSystem(entt::registry& registry);

		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
