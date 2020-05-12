#include "JumpRSystem.h"

#include "../Components/InputComponents.h"
#include "../Components/SimpleComponents.h"
#include "../Utils/Utils.h"

void godot::JumpRSystem::Init(entt::registry& registry)
{
	registry.on_construct<JumpPressedTag>().connect<&JumpRSystem::OnInputPressed>();
}

void godot::JumpRSystem::OnInputPressed(entt::registry& registry, entt::entity entity)
{
	if (registry.has<InAirTag>(entity))
		return;

	ASSERT(registry.has<VelocityComponent>(entity), "entity has no VelocityComponent");
	ASSERT(registry.has<JumpSpeedComponent>(entity), "entity has no JumpSpeedComponent");
	registry.get<VelocityComponent>(entity).velocity.y = registry.get<JumpSpeedComponent>(entity).speed;
}
