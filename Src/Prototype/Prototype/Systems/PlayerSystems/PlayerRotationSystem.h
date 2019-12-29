#pragma once

#include "../BaseSystem.h"
#include "../../Components/SimpleComponents.h"

namespace godot
{
	class TESTABLE_CLASS PlayerRotationSystem : public BaseSystem
	{
	public:
		static Vector3 GetTargetDirection(Vector2 inputDir, Basis camBasis);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
