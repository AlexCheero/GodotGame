#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class ComboDropSystem
	{
	private:
		static void OnMeleeHide(entt::registry& registry, entt::entity entity);
	public:
		static void Init(entt::registry& registry);
		static void Tick(float delta, entt::registry& registry);
	};
}
