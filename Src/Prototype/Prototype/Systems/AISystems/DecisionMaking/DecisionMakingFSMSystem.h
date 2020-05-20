#pragma once

#include <Spatial.hpp>

#include "PatrolStateSystem.h"
#include "PursueStateSystem.h"
#include "MeleeStateSystem.h"
#include "FleeStateSystem.h"

namespace godot
{
	class DecisionMakingFSMSystem
	{
	private:
		static void OnTransitionToPatrol(entt::registry& registry, entt::entity entity);
		static void OnTransitionToPursuing(entt::registry& registry, entt::entity entity);
		static void OnTransitionToFlee(entt::registry& registry, entt::entity entity);
		static void OnTransitionToMelee(entt::registry& registry, entt::entity entity);
		static void OnHitNoticing(entt::registry& registry, entt::entity entity);
	public:
		static void Init(entt::registry& registry);

		static void Tick(float delta, entt::registry& registry);
	};
}
