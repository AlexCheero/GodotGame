#pragma once

#include "../BaseSystem.h"

#include "../../Components/AIComponents/PatrolComponents.h"

namespace godot
{
	class PursuingSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
