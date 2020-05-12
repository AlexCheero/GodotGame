#pragma once

#include "../BaseSystem.h"

#include <PhysicsShapeQueryParameters.hpp>
#include <Spatial.hpp>
#include <SphereShape.hpp>

namespace godot
{
	//TODO: choose hth or melee and use only one prefix
	class HTHLockTargetSystem : public BaseSystem
	{
	private:
		Ref<PhysicsShapeQueryParameters> m_params;
		Ref<SphereShape> m_attackShape;
		
		Array GetIntersects(Spatial* pAttackerSpatial, float distance, String layerName);
	public:
		HTHLockTargetSystem();
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
