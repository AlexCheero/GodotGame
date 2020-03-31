#pragma once

#include "../BaseSystem.h"

#include <PhysicsShapeQueryParameters.hpp>
#include <SphereShape.hpp>
#include <Spatial.hpp>

namespace godot
{
	class GrenadeSystem : public BaseSystem
	{
	private:
		Ref<PhysicsShapeQueryParameters> m_params;
		Ref<SphereShape> m_attackShape;

		bool CheckVisibility(Spatial* pGrenade, Spatial* pTarget, float explosionRadius);
	public:
		GrenadeSystem();

		virtual void operator()(float delta, entt::registry& registry) override;
	};
}