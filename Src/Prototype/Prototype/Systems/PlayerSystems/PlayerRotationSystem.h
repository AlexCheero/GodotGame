#pragma once

#include "../BaseSystem.h"

#include <Basis.hpp>
#include <Vector2.hpp>

namespace godot
{
	class PlayerRotationSystem : public BaseSystem
	{
	private:
		Vector3 GetTargetDirection(Vector2 inputDir, Basis camBasis);
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
