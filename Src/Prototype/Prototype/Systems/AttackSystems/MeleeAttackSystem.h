#pragma once

#include "../BaseSystem.h"
#include "AttackCooldown.h"

#include <PhysicsShapeQueryParameters.hpp>
#include <Spatial.hpp>
#include <SphereShape.hpp>

namespace godot
{
	class TESTABLE_CLASS MeleeAttackSystem : public BaseSystem, private AttackCooldown
	{
		Ref<PhysicsShapeQueryParameters> m_params;
		Ref<SphereShape> m_attackShape;
		Array GetIntersects(Spatial* pAttackerSpatial, float distance);
	public:
		MeleeAttackSystem();
		virtual void operator()(float delta, entt::registry& registry) override;
		static bool CheckAttackAngle(Vector3 attackerPosition, Vector3 attackerDirection, Vector3 targetPosition, float maxAngle);
	};
}
