#pragma once

#include "entt/entt.hpp"

#include <PhysicsShapeQueryParameters.hpp>
#include <SphereShape.hpp>
#include <Spatial.hpp>

namespace godot
{
	class GrenadeSystem
	{
	private:
		static Ref<PhysicsShapeQueryParameters> params;
		static Ref<SphereShape> attackShape;

		static bool CheckVisibility(Spatial* pGrenade, Spatial* pTarget, float explosionRadius);
	public:
		static void Init();

		static void Tick(float delta, entt::registry& registry);
	};
}