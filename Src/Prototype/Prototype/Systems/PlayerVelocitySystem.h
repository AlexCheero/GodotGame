#pragma once

#include <Camera.hpp>

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	//TODO: abstract from player's input
	class TESTABLE_CLASS PlayerVelocitySystem : public BaseSystem
	{
	public:
		void Update(VelocityComponent& velocityComp, SpeedComponent speedComp, Basis camBasis, int directionMask);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
