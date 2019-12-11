#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	//TODO: remove TESTABLE_CLASS macro from not tested classes
	class TESTABLE_CLASS CameraFollowSystem : public BaseSystem
	{
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
