#pragma once

#include "../BaseSystem.h"

#include <Spatial.hpp>

#include "PatrolStateSystem.h"
#include "PursueStateSystem.h"
#include "HTHStateSystem.h"
#include "FleeStateSystem.h"

#include "../../Components/AIComponents/FSMStateComponents.h"

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

		void OnTransitionToPursuing(entt::registry& registry, entt::entity entity);

		void OnHitNoticing(entt::registry& registry);
	public:
		DecisionMakingFSMSystem(entt::registry& registry)
		{
			registry.on_construct<PursuingStateComponent>().connect<&DecisionMakingFSMSystem::OnTransitionToPursuing>(this);
		}

		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
