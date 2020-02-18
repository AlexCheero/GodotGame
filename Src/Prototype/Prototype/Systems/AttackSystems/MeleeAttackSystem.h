#pragma once

#include "../BaseSystem.h"
#include "AttackCooldown.h"

#include <PhysicsShapeQueryParameters.hpp>
#include <Spatial.hpp>
#include <SphereShape.hpp>

#include "../../Components/AttackComponents.h"

namespace godot
{
	class TESTABLE_CLASS MeleeAttackSystem : public BaseSystem, private AttackCooldown
	{
	private:
		Ref<PhysicsShapeQueryParameters> m_params;
		Ref<SphereShape> m_attackShape;
		Array GetIntersects(Spatial* pAttackerSpatial, float distance, String layerName);
		Vector3 GetDirToTarget(Spatial* pAttackerSpatial, MeleeAttackComponent attackComp);
	public:
		MeleeAttackSystem();
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
