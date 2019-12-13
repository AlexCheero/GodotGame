#include "JumpSystem.h"

#include <KinematicBody.hpp>

void godot::JumpSystem::Update(VelocityComponent& velocityComp, JumpSpeedComponent jump)
{
	velocityComp.velocity.y = jump.speed;
}

void godot::JumpSystem::operator()(float delta, entt::registry& registry)
{
	Input* pInput = Input::get_singleton();
	bool is_jump_pressed = pInput->is_action_pressed("jump");
	registry.view<VelocityComponent, JumpSpeedComponent, KinematicBody*>().each([this, is_jump_pressed](VelocityComponent& velocityComp, JumpSpeedComponent jump, KinematicBody* pBody)
	{
		if (pBody->is_on_floor() && is_jump_pressed)
			Update(velocityComp, jump);
	});
}
