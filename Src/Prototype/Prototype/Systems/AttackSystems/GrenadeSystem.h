#pragma once

#include "../BaseSystem.h"

#include "../../Nodes/EntityHolderNode.h"

namespace godot
{
	class GrenadeSystem : public BaseSystem
	{
		bool CheckVisibility(Spatial* pGrenade, EntityHolderNode* pTarget, float explosionRadius);
	public:
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}