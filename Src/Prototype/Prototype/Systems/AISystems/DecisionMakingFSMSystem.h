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

		void OnHitNoticing(entt::registry& registry);
	public:
		DecisionMakingFSMSystem(entt::registry& registry) : patrolSystem(registry) {}
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
