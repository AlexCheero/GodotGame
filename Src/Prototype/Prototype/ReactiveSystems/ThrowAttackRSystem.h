#pragma once

#include "../Systems/BaseSystem.h"

namespace godot
{
	class ThrowAttackSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}