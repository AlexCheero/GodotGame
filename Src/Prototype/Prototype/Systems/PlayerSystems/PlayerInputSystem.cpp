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
	
	InputEventMouseMotion* pMouseEvent = Object::cast_to<InputEventMouseMotion>(e);
	if (pMouseEvent)
	{
		Vector2 relative = pMouseEvent->get_relative();
		relative.y *= -1;
		real_t angle = utils::Rad2deg(relative.angle());

		if (angle < 0)
			angle += 360;
	}

	Vector2 attackDirection = GetInputDirection(pInput, "attack_area");
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