#pragma once

#include <Camera.hpp>

#include "../BaseSystem.h"
#include "../../Components/SimpleComponents.h"

namespace godot
{
	class PlayerVelocitySystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
