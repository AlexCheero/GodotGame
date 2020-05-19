#pragma once

#include "../../BaseSystem.h"

namespace godot
{
	class ComboDropSystem : public BaseSystem
	{
	private:
		void OnMeleeHide(entt::registry& registry, entt::entity entity);
	public:
		ComboDropSystem(entt::registry& registry);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
