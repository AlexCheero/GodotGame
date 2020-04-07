#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	//TODO: try to use SpringArm instead
	class TESTABLE_CLASS SimpleFollowSystem : public BaseSystem
	{
	public:
		static Vector3 NewCamPosition(Vector3 targetPosition, SimpleFollowComponent relPos);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
