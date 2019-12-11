#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	class TESTABLE_CLASS PlayerVelocitySystem : public BaseSystem
	{
	public:
		void Update(VelocityComponent& velocityComp, SpeedComponent speedComp, int directionMask);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
