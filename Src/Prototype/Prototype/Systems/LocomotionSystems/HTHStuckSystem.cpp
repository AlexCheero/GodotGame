#include "HTHStuckSystem.h"

#include "../../Components/AttackComponents.h"

//TODO: remove hardcode
static const float stuckMoveFactor = 0.1f;

void godot::HTHStuckSystem::OnHTHStuckComponentConstruct(entt::registry& registry, entt::entity entity)
{
	registry.get<SpeedComponent>(entity).speed *= stuckMoveFactor;
}

void godot::HTHStuckSystem::OnHTHStuckComponentDestroy(entt::registry& registry, entt::entity entity)
{
	if (registry.has<SpeedComponent>(entity))
		registry.get<SpeedComponent>(entity).speed /= stuckMoveFactor;
}

godot::HTHStuckSystem::HTHStuckSystem(entt::registry& registry)
{
	registry.on_construct<HTHStuckComponent>().connect<&HTHStuckSystem::OnHTHStuckComponentConstruct>(this);
	registry.on_destroy<HTHStuckComponent>().connect<&HTHStuckSystem::OnHTHStuckComponentDestroy>(this);
}

void godot::HTHStuckSystem::operator()(float delta, entt::registry& registry)
{
	auto HTHStuckView = registry.view<HTHStuckComponent>();
	HTHStuckView.each([&registry, delta](entt::entity entity, HTHStuckComponent& comp)
	{
		comp.secondsLeft -= delta;
		if (comp.secondsLeft <= 0)
			registry.remove<HTHStuckComponent>(entity);
	});
}