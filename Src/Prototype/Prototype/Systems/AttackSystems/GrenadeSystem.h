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
		Ref<PhysicsShapeQueryParameters> params;
		Ref<SphereShape> attackShape;

		bool CheckVisibility(Spatial* pGrenade, Spatial* pTarget, float explosionRadius);
	public:
		GrenadeSystem();

		virtual void operator()(float delta, entt::registry& registry) override;
	};
}