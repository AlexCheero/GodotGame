#include "HTHStuckSystem.h"

#include "../../Components/AttackComponents.h"

//TODO: make proper buff/debuff system. speedFactor may vary and thus we get wrong speed after stun is removed
void godot::HTHStuckSystem::OnStunComponentConstruct(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<StunComponent>(entity), "entity has no StunComponent");
	const StunComponent& stunComp = registry.get<StunComponent>(entity);
	registry.get<SpeedComponent>(entity).speed *= stunComp.speedFactor;
}

void godot::HTHStuckSystem::OnStunComponentDestroy(entt::registry& registry, entt::entity entity)
{
	ASSERT(registry.has<StunComponent>(entity), "StunComponent is already deleted");
	const StunComponent& stunComp = registry.get<StunComponent>(entity);
	//TODO: why this check here?
	if (registry.has<SpeedComponent>(entity))
		registry.get<SpeedComponent>(entity).speed /= stunComp.speedFactor;
}

godot::HTHStuckSystem::HTHStuckSystem(entt::registry& registry)
{
	registry.on_construct<StunComponent>().connect<&HTHStuckSystem::OnStunComponentConstruct>(this);
	registry.on_destroy<StunComponent>().connect<&HTHStuckSystem::OnStunComponentDestroy>(this);
}

void godot::HTHStuckSystem::operator()(float delta, entt::registry& registry)
{
	auto HTHStuckView = registry.view<StunComponent>();
	HTHStuckView.each([&registry, delta](entt::entity entity, StunComponent& comp)
	{
		comp.secondsLeft -= delta;
		if (comp.secondsLeft <= 0)
			registry.remove<StunComponent>(entity);
	});
}