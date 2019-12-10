#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	class TESTABLE_CLASS PlayerFlatVelocitySystem : public BaseSystem
	{
	public:
		void Update(FlatVelocityComponent& movement, SpeedComponent& speed, float delta, int directionMask);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
