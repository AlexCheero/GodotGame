#include "JumpSystem.h"

#include <KinematicBody.hpp>
#include <Input.hpp>

#include "../Components/InputComponents.h"

void godot::JumpSystem::Update(VelocityComponent& velocityComp, JumpSpeedComponent jump)
{
	velocityComp.velocity.y = jump.speed;
}

void godot::JumpSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<VelocityComponent, JumpSpeedComponent, InputComponent, KinematicBody*>();
	view.each([&registry](entt::entity entity, VelocityComponent& velocityComp, JumpSpeedComponent jump, InputComponent comp, KinematicBody* pBody)
	{
		if (!comp.Test(EInput::Jump) || !pBody->is_on_floor())
			return;
		
		Update(velocityComp, jump);
	});
}
