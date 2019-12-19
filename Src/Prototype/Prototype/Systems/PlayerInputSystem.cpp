#include "PlayerInputSystem.h"

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
	//TODO: move all input outside of lambda, to int mask
	registry.view<entt::tag<PlayerInputTag> >().each([&registry, pInput](entt::entity entity, entt::tag<PlayerInputTag> comp)
	{
		if (pInput->is_action_pressed("attack") && !registry.has<entt::tag<AttackedInputTag> >(entity))
			registry.assign<entt::tag<AttackedInputTag> >(entity);
		if (pInput->is_action_pressed("jump") && !registry.has<entt::tag<JumpedInputTag> >(entity))
			//TODO: fix double jump (assign in proc and remove in physicProc leads to remove 1st and instant assign of the 2nd jump)
			registry.assign<entt::tag<JumpedInputTag> >(entity);
		
		registry.get<InputRotationComponent&>(entity).dir = GetInputDirection(pInput, "ui");
		registry.get<InputVelocityComponent&>(entity).dir = GetInputDirection(pInput, "move");
		
	});
}