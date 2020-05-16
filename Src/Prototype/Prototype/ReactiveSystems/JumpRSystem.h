#pragma once

#include "entt/entt.hpp"

#include "../Systems/BaseSystem.h"

namespace godot
{
	class JumpSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
