#include "PlayerInputSystem.h"

#include <InputEventMouseMotion.hpp>

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

template<typename T>
void godot::PlayerInputSystem::ProcessJustPressedKey(entt::registry& registry, Input* pInput, String action)
{
	if (!pInput->is_action_just_pressed(action))
		return;
	auto view = registry.view<PlayerTag>(entt::exclude<T>);
	registry.insert<T>(view.begin(), view.end());
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

void godot::PlayerInputSystem::HandleInput(entt::registry& registry, InputEvent* e)
{
	godot::Input* pInput = godot::Input::get_singleton();

	ProcessJustPressedKey<AttackPressedTag>(registry, pInput, "attack");
	ProcessJustPressedKey<JumpPressedTag>(registry, pInput, "jump");
	ProcessJustPressedKey<ChooseMeleePressedTag>(registry, pInput, "choose_melee");
	ProcessJustPressedKey<ChooseRangedPressedTag>(registry, pInput, "choose_ranged");
	ProcessJustPressedKey<ChooseThrowablePressedTag>(registry, pInput, "choose_throwable");
	ProcessJustPressedKey<ChooseGrenadePressedTag>(registry, pInput, "choose_grenade");

	ProcessInputAxis<MoveDirInputComponent>(registry, GetInputDirection(pInput, "move"));
	ProcessInputAxis<RotationInputComponent>(registry, GetInputDirection(pInput, "rotate"));

	Vector2 attackDirection = GetInputDirection(pInput, "attack_area");

	//TODO0: check if mouse used as attack input
	//TODO0: and also should check if in melee mode no difference mouse or joystick mode
	//TODO0: try to assign if direction.length > 0 and remove check from PlayerAttackInputSystem::Tick
	InputEventMouseMotion* pMouseEvent = Object::cast_to<InputEventMouseMotion>(e);
	if (pMouseEvent)
	{
		attackDirection += pMouseEvent->get_relative();
		attackDirection.y *= -1;
	}

	bool altAttack = pInput->is_action_pressed("alt_melee_attack");
	bool legAttack = pInput->is_action_pressed("leg_melee_attack");
	auto attackInputView = registry.view<PlayerTag, AttackInputComponent>();
	attackInputView.each([attackDirection, altAttack, legAttack](AttackInputComponent& inputComp)
	{
		inputComp.dir = attackDirection;
		inputComp.alt = altAttack;
		inputComp.leg = legAttack;
	});
}