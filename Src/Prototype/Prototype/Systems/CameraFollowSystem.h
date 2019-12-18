#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	//TODO: abstract from camera
	class TESTABLE_CLASS CameraFollowSystem : public BaseSystem
	{
	public:
		static Vector3 NewCamPosition(Vector3 targetPosition, CamPositionComponent relPos);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
