#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class PileInSystem
	{
	private:
		static void OnPileInTagDestroyed(entt::registry& registry, entt::entity entity);
	public:
		static void Init(entt::registry& registry);
		static void Tick(float delta, entt::registry& registry);
	};
}
