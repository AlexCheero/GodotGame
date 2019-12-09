#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

#include <KinematicBody.hpp>

namespace godot
{
	class KinematicMovementSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
