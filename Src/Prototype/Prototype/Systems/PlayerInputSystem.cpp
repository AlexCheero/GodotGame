#include "PlayerInputSystem.h"

#include <Input.hpp>

void godot::PlayerInputSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();

	//TODO: read once more about differences between groups and view and, probably, use group instead
	registry.view<entt::tag<PlayerInputTag> >().each([&registry, pInput](entt::entity entity, entt::tag<PlayerInputTag> comp)
	{
		if (pInput->is_action_pressed("attack") && !registry.has<entt::tag<AttackedInputTag> >(entity))
			registry.assign<entt::tag<AttackedInputTag> >(entity);
		if (pInput->is_action_pressed("jump") && !registry.has<entt::tag<JumpedInputTag> >(entity))
			registry.assign<entt::tag<JumpedInputTag> >(entity);
	});
}