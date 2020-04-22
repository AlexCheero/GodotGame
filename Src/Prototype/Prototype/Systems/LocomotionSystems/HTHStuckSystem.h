#pragma once

#include "../BaseSystem.h"

namespace godot
{
	class HTHStuckSystem : public BaseSystem
	{
	private:
		void OnStunComponentConstruct(entt::registry& registry, entt::entity entity);
		void OnStunComponentDestroy(entt::registry& registry, entt::entity entity);
	public:
		HTHStuckSystem(entt::registry& registry);
		virtual void operator()(float delta, entt::registry& registry) override;
	};
}
