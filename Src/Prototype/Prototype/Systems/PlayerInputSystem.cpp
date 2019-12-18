#include "PlayerInputSystem.h"

#include <Input.hpp>

void godot::PlayerInputSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();

	//TODO: read once more about differences between groups and view and, probably, use group instead
	//TODO: move all input outside of lambda, to int mask
	registry.view<entt::tag<PlayerInputTag> >().each([&registry, pInput](entt::entity entity, entt::tag<PlayerInputTag> comp)
	{
		if (pInput->is_action_pressed("attack") && !registry.has<entt::tag<AttackedInputTag> >(entity))
			registry.assign<entt::tag<AttackedInputTag> >(entity);
		if (pInput->is_action_pressed("jump") && !registry.has<entt::tag<JumpedInputTag> >(entity))
			registry.assign<entt::tag<JumpedInputTag> >(entity);
		
		bool rotateLeft = pInput->is_action_pressed("ui_left");
		bool rotateRight = pInput->is_action_pressed("ui_right");
		bool rotateUp = pInput->is_action_pressed("ui_up");
		bool rotateDown = pInput->is_action_pressed("ui_down");
		bool rotated = rotateLeft || rotateRight || rotateUp || rotateDown;
		if (rotated && !registry.has<InputDirectionComponent>(entity))
		{
			InputDirectionComponent& component = registry.assign<InputDirectionComponent>(entity, Vector2{ 0.f, 0.f });
			if (rotateLeft)
				component.dir.x = 1;
			if (rotateRight)
				component.dir.x = -1;
			if (rotateUp)
				component.dir.y = 1;
			if (rotateDown)
				component.dir.y = -1;
		}
	});
}