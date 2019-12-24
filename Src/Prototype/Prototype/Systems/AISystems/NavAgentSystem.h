#pragma once

#include "../BaseSystem.h"

namespace godot
{
	class NavAgentSystem : BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
