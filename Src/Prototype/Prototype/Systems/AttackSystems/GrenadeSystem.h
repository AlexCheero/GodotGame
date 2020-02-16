#pragma once

#include "../BaseSystem.h"

#include <PhysicsShapeQueryParameters.hpp>
#include <SphereShape.hpp>

#include "../../Nodes/EntityHolderNode.h"

namespace godot
{
	class GrenadeSystem : public BaseSystem
	{
	private:
		Ref<PhysicsShapeQueryParameters> m_params;
		Ref<SphereShape> m_attackShape;

		bool CheckVisibility(Spatial* pGrenade, EntityHolderNode* pTarget, float explosionRadius);
	public:
		GrenadeSystem();

		virtual void operator()(float delta, entt::registry& registry) override;
	};
}