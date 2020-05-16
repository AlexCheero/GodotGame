#pragma once

#include "../Systems/BaseSystem.h"

#include <Spatial.hpp>
#include <PhysicsShapeQueryParameters.hpp>
#include <SphereShape.hpp>

namespace godot
{
	//TODO: choose hth or melee and use only one prefix
	class HTHLockTargetSystem : public BaseSystem
	{
	private:
		const float INTERSECT_RESULTS_NUM = 16.f;

		Ref<PhysicsShapeQueryParameters> m_params;
		Ref<SphereShape> m_attackShape;

		Array GetIntersects(Spatial* pAttackerSpatial, float distance, String layerName);
	public:
		HTHLockTargetSystem();
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
