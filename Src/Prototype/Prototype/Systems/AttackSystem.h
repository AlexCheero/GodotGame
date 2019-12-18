#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

#include <PhysicsShapeQueryParameters.hpp>
#include <Spatial.hpp>
#include <SphereShape.hpp>

namespace godot
{
	//TODO: rename to PlayerAttackSystem or abstract player input somehow
	class TESTABLE_CLASS AttackSystem : public BaseSystem
	{
		Ref<PhysicsShapeQueryParameters> m_params;
		Ref<SphereShape> m_attackShape;
		Array GetIntersects(Spatial* pAttackerSpatial, float distance);
	public:
		AttackSystem();
		virtual void operator()(float delta, entt::registry& registry) override;
		static bool CheckAttackAngle(Vector3 attackerPosition, Vector3 attackerDirection, Vector3 targetPosition, float maxAngle);
	};
}
