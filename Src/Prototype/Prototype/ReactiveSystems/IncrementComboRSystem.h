#pragma once

#include "../Systems/BaseSystem.h"

namespace godot
{
	class IncrementComboSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
