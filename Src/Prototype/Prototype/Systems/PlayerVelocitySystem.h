#pragma once

#include <Camera.hpp>

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	//TODO: try to abstract from player
	class TESTABLE_CLASS PlayerVelocitySystem : public BaseSystem
	{
	public:
		static void Update(VelocityComponent& velocityComp, SpeedComponent speedComp, Basis camBasis, Vector2 input);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
