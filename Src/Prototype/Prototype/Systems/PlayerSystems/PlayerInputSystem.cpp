#include "PlayerInputSystem.h"

#include "../../Components/InputComponents.h"
#include "../../Components/SimpleComponents.h"

inline void godot::PlayerInputSystem::GetInputDirection(Vector2& dir, InputEvent* e, const char* actionPrefix)
{
	if (e->is_action_pressed(actionPrefix + String("_left")))
		dir.x = -1;
	else if (e->is_action_released(actionPrefix + String("_left")))
		dir.x = 0;

	if (e->is_action_pressed(actionPrefix + String("_right")))
		dir.x = 1;
	else if (e->is_action_released(actionPrefix + String("_right")))
		dir.x = 0;

	if (e->is_action_pressed(actionPrefix + String("_up")))
		dir.y = -1;
	else if (e->is_action_released(actionPrefix + String("_up")))
		dir.y = 0;

	if (e->is_action_pressed(actionPrefix + String("_down")))
		dir.y = 1;
	else if (e->is_action_released(actionPrefix + String("_down")))
		dir.y = 0;
}

void godot::PlayerInputSystem::operator()(entt::registry& registry, InputEvent* e)
{
	//TODO: read once more about differences between groups and view and, probably, use group instead
	//TODO: implement proper pressed/just_pressed functional
	auto view = registry.view<entt::tag<PlayerTag>, InputComponent>();
	view.less([&registry, e](InputComponent& comp)
	{
		if (e->is_action_pressed("attack"))
			comp.Set(EInput::Attack, true);
		else if (e->is_action_released("attack"))
			comp.Set(EInput::Attack, false);

		if (e->is_action_pressed("jump"))
			comp.Set(EInput::Jump, true);
		else if (e->is_action_released("jump"))
			comp.Set(EInput::Jump, false);

		if (e->is_action_pressed("choose_melee"))
		{
			comp.Set(EInput::ChooseMelee, true);
			comp.Set(EInput::ChooseRanged, false);
			comp.Set(EInput::ChooseThrowable, false);
		}
		if (e->is_action_pressed("choose_ranged"))
		{
			comp.Set(EInput::ChooseMelee, false);
			comp.Set(EInput::ChooseRanged, true);
			comp.Set(EInput::ChooseThrowable, false);
		}
		if (e->is_action_pressed("choose_throwable"))
		{
			comp.Set(EInput::ChooseMelee, false);
			comp.Set(EInput::ChooseRanged, false);
			comp.Set(EInput::ChooseThrowable, true);
		}

		//TODO: change to mouse
		GetInputDirection(comp.rotation, e, "ui");
		GetInputDirection(comp.moveDir, e, "move");
	});
}