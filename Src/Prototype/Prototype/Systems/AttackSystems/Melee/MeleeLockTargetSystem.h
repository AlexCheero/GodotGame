#pragma once

#include "entt/entt.hpp"

#include <Spatial.hpp>
#include <PhysicsShapeQueryParameters.hpp>
#include <SphereShape.hpp>

namespace godot
{
	class MeleeLockTargetSystem
	{
	private:
		static constexpr float INTERSECT_RESULTS_NUM = 16.f;

		static Ref<PhysicsShapeQueryParameters> params;
		static Ref<SphereShape> attackShape;

		static Array GetIntersects(Spatial* pAttackerSpatial, float distance, String layerName);
	public:
		static void Init();
		static void Tick(float delta, entt::registry& registry);
	};
}
