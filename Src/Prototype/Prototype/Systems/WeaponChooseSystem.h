#pragma once

#include "BaseSystem.h"

namespace godot
{
	class WeaponChooseSystem : public BaseSystem
	{
	public:
		static void OnMeleeTagConstruct(entt::registry& registry, entt::entity entity);
		static void OnCastTagConstruct(entt::registry& registry, entt::entity entity);
		static void OnThrowableTagConstruct(entt::registry& registry, entt::entity entity);

		virtual void operator()(float delta, entt::registry& registry) override;
	};
}