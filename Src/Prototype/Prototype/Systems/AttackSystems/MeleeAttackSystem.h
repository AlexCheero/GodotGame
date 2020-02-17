#pragma once

#include "../BaseSystem.h"
#include "AttackCooldown.h"

#include <PhysicsShapeQueryParameters.hpp>
#include <Spatial.hpp>
#include <SphereShape.hpp>

namespace godot
{
	//TODO: rename to reactive system or just turn it into free function
	class TESTABLE_CLASS MeleeAttackSystem
	{
	private:
		static Ref<PhysicsShapeQueryParameters> m_params;
		static Ref<SphereShape> m_attackShape;
		static Array GetIntersects(Spatial* pAttackerSpatial, float distance, String layerName);
	public:
		static void InitParams();
		static void OnMeleeInputTagConstruct(entt::registry& registry, entt::entity entity);
		static bool CheckAttackAngle(Vector3 attackerPosition, Vector3 attackerDirection, Vector3 targetPosition, float maxAngle);
	};
}
