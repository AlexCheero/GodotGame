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
		static bool MatchPattern(const AttackInputAggregatorComponent::AggregatorType& aggregation, const std::vector<float>& pattern);
	public:
		static std::vector<float> angles;
		static int64_t patternMatchingTime;

		static void Tick(float delta, entt::registry& registry);
	};
}
