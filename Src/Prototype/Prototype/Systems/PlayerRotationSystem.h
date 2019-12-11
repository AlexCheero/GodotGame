#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	class TESTABLE_CLASS PlayerRotationSystem : public BaseSystem
	{
	public:
		//void Update(RotationComponent& velocityComp, int directionMask);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
