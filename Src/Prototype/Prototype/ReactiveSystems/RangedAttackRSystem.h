#pragma once

#include "../Systems/BaseSystem.h"

namespace godot
{
	class RangedAttackSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}