#pragma once

#include "../BaseSystem.h"

namespace godot
{
	class EndAttackAnimSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
