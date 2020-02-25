#pragma once

#include "../BaseSystem.h"
#include "AttackCooldown.h"

#include <PhysicsShapeQueryParameters.hpp>
#include <Spatial.hpp>
#include <SphereShape.hpp>
//TODO: remove if hth anim will be played in HTHAnimSystem
#include <AnimationTree.hpp>

#include "../../Components/AttackComponents.h"

namespace godot
{
	class TESTABLE_CLASS MeleeAttackSystem : public BaseSystem, private AttackCooldown
	{
	private:
		Ref<PhysicsShapeQueryParameters> m_params;
		Ref<SphereShape> m_attackShape;
		Array GetIntersects(Spatial* pAttackerSpatial, float distance, String layerName);
		Vector3 GetDirToTarget(Spatial* pAttackerSpatial, entt::registry& registry, entt::entity lockedTarget);
		bool ChecktargetEntity(entt::registry& registry, entt::entity lockedTarget);
		void PlayAnim(entt::entity entity, InputComponent& input, MeleeAttackComponent& attackComp, AnimationTree* pAnimTree);
	public:
		MeleeAttackSystem();
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
