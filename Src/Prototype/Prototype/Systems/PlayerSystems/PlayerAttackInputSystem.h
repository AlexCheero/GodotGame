#pragma once

#include "entt/entt.hpp"

#include <Vector2.hpp>

#include <unordered_map>
#include <string>

#include "../../Components/InputComponents.h"

namespace std
{
    template <>
    struct hash<godot::String>
    {
        std::size_t operator()(const godot::String& str) const
        {
            return std::hash<std::string>()(str.alloc_c_string());
        }
    };

}

namespace godot
{
	class PlayerAttackInputSystem
	{
	private:
		//TODO: try to use godot internal or binding containers instead
		static std::unordered_map<String, std::vector<float>> patterns;

		static float ClampInputAngle(Vector2 dir);
		static bool MatchPattern(AttackInputAggregatorComponent::AggregatorType aggregation, std::vector<float> pattern);
	public:
		static void Init();
		static void Tick(float delta, entt::registry& registry);
	};
}
