#pragma once

#include "../../BaseSystem.h"

#include <Spatial.hpp>
#include <PhysicsShapeQueryParameters.hpp>
#include <SphereShape.hpp>

namespace godot
{
	class MeleeLockTargetSystem : public BaseSystem
	{
	private:
		const float INTERSECT_RESULTS_NUM = 16.f;

		Ref<PhysicsShapeQueryParameters> m_params;
		Ref<SphereShape> m_attackShape;

		Array GetIntersects(Spatial* pAttackerSpatial, float distance, String layerName);
	public:
		MeleeLockTargetSystem();
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
