#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

namespace godot
{
	class TESTABLE_CLASS PlayerRotationSystem : public BaseSystem
	{
	private:
		Vector2 GetInputDir(int directionMask);
	public:
		Vector3 GetTargetDirection(Vector2 inputDir, Basis camBasis);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
