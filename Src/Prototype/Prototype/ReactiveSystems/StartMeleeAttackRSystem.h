#pragma once

#include "entt/entt.hpp"

#include "../Systems/BaseSystem.h"

namespace godot
{
	class StartMeleeAttackSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
