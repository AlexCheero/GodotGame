#include "PlayerInputSystem.h"

#include <Input.hpp>

#include "../../Components/InputComponents.h"

namespace //private
{
	template<typename T>
	inline void ProcessInput(entt::registry& registry, godot::InputEvent* e, bool pressed)
	{
		if (pressed)
		{
			auto view = registry.view<PlayerTag>(entt::exclude<T>);
			registry.assign<T>(view.begin(), view.end());
		}
		else
		{
			auto view = registry.view<PlayerTag, T>();
			registry.remove<T>(view.begin(), view.end());
		}
	}

	godot::Vector2 GetInputDirection(const char* actionPrefix)
	{
		godot::Input* pInput = godot::Input::get_singleton();
		float horizontal = pInput->get_action_strength(actionPrefix + godot::String("_right")) - pInput->get_action_strength(actionPrefix + godot::String("_left"));
		float vertical = pInput->get_action_strength(actionPrefix + godot::String("_up")) - pInput->get_action_strength(actionPrefix + godot::String("_down"));

		return { horizontal, vertical };
	}
}

void godot::PlayerInputSystem::HandleInput(entt::registry& registry, InputEvent* e)
{
	ProcessInput<AttackPressedTag>(registry, e, e->is_action_pressed("attack"));
	ProcessInput<JumpPressedTag>(registry, e, e->is_action_pressed("jump"));
	ProcessInput<ChooseMeleePressedTag>(registry, e, e->is_action_pressed("choose_melee"));
	ProcessInput<ChooseRangedPressedTag>(registry, e, e->is_action_pressed("choose_ranged"));
	ProcessInput<ChooseThrowablePressedTag>(registry, e, e->is_action_pressed("choose_throwable"));

	auto view = registry.view<PlayerTag, InputComponent>();
	view.less([&registry, e](InputComponent& comp)
	{
		//TODO: change to mouse and/or gamepad
		comp.rotation = GetInputDirection("ui");
		comp.moveDir = GetInputDirection("move");
	});
}