#pragma once

#include "../BaseSystem.h"

namespace godot
{
	class PatrolStateSystem : public BaseSystem
	{
	private:
		entt::observer toPursuitTransitionObserver;
	public:
		PatrolStateSystem(entt::registry& registry);

		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
