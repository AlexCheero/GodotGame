#pragma once

#include "../BaseSystem.h"

#include <Vector2.hpp>
#include <Basis.hpp>

namespace godot
{
	class PlayerVelocitySystem : public BaseSystem
	{
	private:
		Vector3 GetTargetDirection(Vector2 inputDir, Basis camBasis);
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}