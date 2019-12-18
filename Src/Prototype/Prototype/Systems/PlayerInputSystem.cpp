#include "PlayerInputSystem.h"

#include <Input.hpp>

void godot::PlayerInputSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();

	//TODO: read once more about differences between groups and view and, probably, use group instead
	registry.view<PlayerInputTag>().each([&registry, pInput](entt::entity entity, PlayerInputTag comp)
	{
		if (pInput->is_action_pressed("attack") && !registry.has<AttackedInputTag>(entity))
			registry.assign<AttackedInputTag>(entity);
		if (pInput->is_action_pressed("jump") && !registry.has<JumpedInputTag>(entity))
			registry.assign<JumpedInputTag>(entity);
	});
}