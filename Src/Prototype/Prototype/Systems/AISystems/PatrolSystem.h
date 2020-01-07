#pragma once

#include "../BaseSystem.h"

#include <Spatial.hpp>

#include "../../Components/SimpleComponents.h"

namespace godot
{
	class PatrolSystem : public BaseSystem
	{
	private:
		using PlayersView = entt::view<entt::exclude_t<>, entt::tag<PlayerTag>, Spatial*>;
		
		bool CheckForTargets(PlayersView& targetsView, Spatial* pPatrolSpatial);
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
