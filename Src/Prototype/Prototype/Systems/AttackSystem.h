#pragma once

#include "BaseSystem.h"
#include "../Components/SimpleComponents.h"

#include <PhysicsShapeQueryParameters.hpp>

namespace godot
{
	//TODO: rename to PlayerAttackSystem or abstract player input somehow
	//TODO: test or remove testable
	class TESTABLE_CLASS AttackSystem : public BaseSystem
	{
		Ref<PhysicsShapeQueryParameters> m_params;
	public:
		AttackSystem();
		virtual void operator()(float delta, entt::registry& registry) override;
		bool CheckAttackAngle(Transform attackerTransform, Vector3 enemyPosition, float maxAngle);
	};
}
