#pragma once

#include "../BaseSystem.h"

namespace godot
{
	class PileInSystem : public BaseSystem
	{
	private:
		void OnPileInTagDestroyed(entt::registry& registry, entt::entity entity);
	public:
		PileInSystem(entt::registry& registry);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
