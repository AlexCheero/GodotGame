#include "PlayerInputSystem.h"

#include <Input.hpp>

#include "../../Components/InputComponents.h"
#include "../../Components/SimpleComponents.h"

inline godot::Vector2 godot::PlayerInputSystem::GetInputDirection(const char* actionPrefix)
{
	Input* pInput = Input::get_singleton();
	float horizontal = pInput->get_action_strength(actionPrefix + String("_right")) - pInput->get_action_strength(actionPrefix + String("_left"));
	float vertical = pInput->get_action_strength(actionPrefix + String("_up")) - pInput->get_action_strength(actionPrefix + String("_down"));

	return { horizontal, vertical };
}

void godot::PlayerInputSystem::operator()(entt::registry& registry, InputEvent* e)
{
	//TODO: read once more about differences between groups and view and, probably, use group instead
	auto view = registry.view<entt::tag<PlayerTag>, InputComponent>();
	view.less([&registry, e](InputComponent& comp)
	{
		comp.Set(EInput::Attack, e->is_action_pressed("attack"));
		comp.Set(EInput::Jump, e->is_action_pressed("jump"));
		comp.Set(EInput::ChooseMelee, e->is_action_pressed("choose_melee"));
		comp.Set(EInput::ChooseRanged, e->is_action_pressed("choose_ranged"));
		comp.Set(EInput::ChooseThrowable, e->is_action_pressed("choose_throwable"));

		//TODO: change to mouse
		comp.rotation = GetInputDirection("ui");
		comp.moveDir = GetInputDirection("move");
	});
}