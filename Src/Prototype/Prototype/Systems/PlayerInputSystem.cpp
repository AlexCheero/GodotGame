#include "PlayerInputSystem.h"

#include <Input.hpp>

void godot::PlayerInputSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();

	//TODO: read once more about differences between groups and view and, probably, use group instead
	auto view = registry.view<PlayerInputTag>(entt::exclude<AttackedInputTag>);
	view.each([&registry, pInput](entt::entity entity, PlayerInputTag comp)
	{
		if (pInput->is_action_pressed("attack"))
			registry.assign<AttackedInputTag>(entity);
	});
}