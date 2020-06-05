#include "PlayerInputSystem.h"

#include "../../Components/InputComponents.h"
#include "../../Utils/Utils.h"

template<typename T>
void godot::PlayerInputSystem::ProcessInputKey(entt::registry& registry, bool pressed)
	{
		if (pressed)
		{
			auto view = registry.view<PlayerTag>(entt::exclude<T>);
			registry.insert<T>(view.begin(), view.end());
		}
		else
		{
			auto view = registry.view<PlayerTag, T>();
			registry.remove<T>(view.begin(), view.end());
		}
	}

godot::Vector2 godot::PlayerInputSystem::GetInputDirection(godot::Input* pInput, godot::String actionPrefix)
	{
		float horizontal = pInput->get_action_strength(actionPrefix + "_right") - pInput->get_action_strength(actionPrefix + "_left");
		float vertical = pInput->get_action_strength(actionPrefix + "_up") - pInput->get_action_strength(actionPrefix + "_down");

		return { horizontal, vertical };
	}

template<typename T>
void godot::PlayerInputSystem::ProcessInputAxis(entt::registry& registry, godot::Vector2 direction)
	{
		auto view = registry.view<PlayerTag, T>();
		view.each([direction](T& inputComp) { inputComp.dir = direction; });
	}

void godot::PlayerInputSystem::HandleInput(entt::registry& registry)
{
	godot::Input* pInput = godot::Input::get_singleton();

	ProcessInputKey<AttackPressedTag>(registry, pInput->is_action_pressed("attack"));
	ProcessInputKey<JumpPressedTag>(registry, pInput->is_action_pressed("jump"));
	ProcessInputKey<ChooseMeleePressedTag>(registry, pInput->is_action_pressed("choose_melee"));
	ProcessInputKey<ChooseRangedPressedTag>(registry, pInput->is_action_pressed("choose_ranged"));
	ProcessInputKey<ChooseThrowablePressedTag>(registry, pInput->is_action_pressed("choose_throwable"));
	ProcessInputKey<ChooseGrenadePressedTag>(registry, pInput->is_action_pressed("choose_grenade"));

	ProcessInputAxis<MoveDirInputComponent>(registry, GetInputDirection(pInput, "move"));
	ProcessInputAxis<RotationInputComponent>(registry, GetInputDirection(pInput, "rotate"));
}