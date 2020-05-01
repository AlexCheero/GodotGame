#pragma once

#include "BaseSystem.h"

namespace godot
{
	class WeaponChooseSystem : public BaseSystem
	{
	private:
		void OnMeleeAttackComponentAssign(entt::registry& registry, entt::entity entity);
		void OnRangedAttackComponentAssign(entt::registry& registry, entt::entity entity);
		void OnThrowableAttackComponentAssign(entt::registry& registry, entt::entity entity);

		void OnMeleeTagConstruct(entt::registry& registry, entt::entity entity);
		void OnRangedTagConstruct(entt::registry& registry, entt::entity entity);
		void OnThrowableTagConstruct(entt::registry& registry, entt::entity entity);
	public:
		WeaponChooseSystem(entt::registry& registry);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}