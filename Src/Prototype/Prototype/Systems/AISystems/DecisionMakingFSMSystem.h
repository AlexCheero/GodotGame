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
		using PlayersView = entt::view<entt::exclude_t<>, entt::tag<PlayerTag>, Spatial*>;

		bool CanSeeTarget(PlayersView& targetsView, entt::entity targetEntity, PatrolmanComponent patrolman, Spatial* pPatrolSpatial);
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
