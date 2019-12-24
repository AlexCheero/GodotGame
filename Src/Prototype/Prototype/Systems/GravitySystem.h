#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	//TODO: move all locomotion systems to separate filter/folder
	class TESTABLE_CLASS GravitySystem : public BaseSystem
	{
	public:
		static void Update(VelocityComponent& velocityComp, GravityComponent& gravity, float delta, bool isOnFloor);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
