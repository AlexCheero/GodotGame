#pragma once

#include "entt/entt.hpp"

#include <Vector2.hpp>

#include "../../Components/InputComponents.h"

namespace godot
{
	class PlayerAttackInputSystem
	{
	private:
		static float ClampInputAngle(Vector2 dir);
		static bool MatchPattern(AttackInputAggregatorComponent::AggregatorType aggregation, std::vector<float> pattern);
	public:
		static void Tick(float delta, entt::registry& registry);
	};
}
