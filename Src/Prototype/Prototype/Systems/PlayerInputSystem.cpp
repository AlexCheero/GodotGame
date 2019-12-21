#include "PlayerInputSystem.h"

#include "../Components/InputComponents.h"

inline godot::Vector2 godot::PlayerInputSystem::GetInputDirection(Input* pInput, const char* actionPrefix)
{
	Vector2 direction{ 0.f, 0.f };
	if (pInput->is_action_pressed(actionPrefix + String("_left")))
		direction.x += 1;
	if (pInput->is_action_pressed(actionPrefix + String("_right")))
		direction.x -= 1;
	if (pInput->is_action_pressed(actionPrefix + String("_up")))
		direction.y += 1;
	if (pInput->is_action_pressed(actionPrefix + String("_down")))
		direction.y -= 1;

	return direction;
}

//TODO: update in ECSWorld's input callback
void godot::PlayerInputSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();

	//TODO: read once more about differences between groups and view and, probably, use group instead
	registry.view<InputComponent, entt::tag<PlayerInputTag> >().each([&registry, pInput](entt::entity entity, InputComponent& comp, entt::tag<PlayerInputTag> tag)
	{
		comp.Set(EInput::Attack, pInput->is_action_pressed("attack"));
		comp.Set(EInput::Jump, pInput->is_action_pressed("jump"));
		
		comp.rotation = GetInputDirection(pInput, "ui");
		comp.moveDir = GetInputDirection(pInput, "move");
	});
}