#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	extern "C"
	{
		class PlayerFlatVelocitySystem : public BaseSystem
		{
		public:
			__declspec(dllexport) void Update(FlatVelocityComponent& movement, SpeedComponent& speed, float delta, int directionMask);
			__declspec(dllexport) virtual void operator()(float delta, entt::registry& registry) override;
		};
	}
}
