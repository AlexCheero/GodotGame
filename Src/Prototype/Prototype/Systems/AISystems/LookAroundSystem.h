#pragma once

#include "../BaseSystem.h"

#include <Spatial.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/PatrolComponents.h"

namespace godot
{
	class LookAroundSystem : public BaseSystem
	{
	private:
		using PlayersView = entt::view<entt::exclude_t<>, entt::tag<PlayerTag>, BoundsComponent, Spatial*>;

		bool CanSeeTarget(PlayersView& targetsView, entt::entity targetEntity, PatrolmanComponent patrolman, Spatial* pPatrolSpatial, float navAgentRadius);
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
