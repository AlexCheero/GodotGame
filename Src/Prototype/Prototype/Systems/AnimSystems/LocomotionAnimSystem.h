#pragma once

#include "../BaseSystem.h"

namespace godot
{
	class TESTABLE_CLASS LocomotionAnimSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
