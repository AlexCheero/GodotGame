#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	class TESTABLE_CLASS GravitySystem : public BaseSystem
	{
	public:
		void Update(VelocityComponent& velocity, GravityComponent& gravity, float delta, bool isOnFloor);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
