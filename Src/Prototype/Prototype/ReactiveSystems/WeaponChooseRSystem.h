#pragma once

#include "entt/entt.hpp"

namespace godot
{
	class WeaponChooseRSystem
	{
	private:
		static void OnMeleeChoose(entt::registry& registry, entt::entity entity);
		static void OnRangedChoose(entt::registry& registry, entt::entity entity);
		static void OnThrowableChoose(entt::registry& registry, entt::entity entity);
		static void OnGrenadeChoose(entt::registry& registry, entt::entity entity);
		static void OnMeleeAttackComponentAssign(entt::registry& registry, entt::entity entity);
		static void SwitchToMeleeOnWeaponThrow(entt::registry& registry, entt::entity entity);
		static void OnRangedAttackComponentAssign(entt::registry& registry, entt::entity entity);
		static void OnThrowableAttackComponentAssign(entt::registry& registry, entt::entity entity);
		static void OnGrenadeAttackComponentAssign(entt::registry& registry, entt::entity entity);
		static void OnMeleeTagConstruct(entt::registry& registry, entt::entity entity);
		static void OnRangedTagConstruct(entt::registry& registry, entt::entity entity);
		static void OnThrowableTagConstruct(entt::registry& registry, entt::entity entity);
		static void OnGrenadeTagConstruct(entt::registry& registry, entt::entity entity);
	public:
		static void Init(entt::registry& registry);
	};
}