#pragma once

#include "../../BaseSystem.h"

namespace godot
{
	class FleeStateSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
