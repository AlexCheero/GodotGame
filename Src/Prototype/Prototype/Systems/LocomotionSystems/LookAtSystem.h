#pragma once

#include "../BaseSystem.h"
#include "../../Components/SimpleComponents.h"

namespace godot
{
	class LookAtSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
