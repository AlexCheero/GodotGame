#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	class TESTABLE_CLASS JumpSystem : public BaseSystem
	{
	public:
		void Update(VelocityComponent& velocityComp, JumpSpeedComponent jump);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
