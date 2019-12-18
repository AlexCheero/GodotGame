#include "JumpSystem.h"

#include <KinematicBody.hpp>
#include <Input.hpp>

void godot::JumpSystem::Update(VelocityComponent& velocityComp, JumpSpeedComponent jump)
{
	velocityComp.velocity.y = jump.speed;
}

void godot::JumpSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<VelocityComponent, JumpSpeedComponent, entt::tag<JumpedInputTag>, KinematicBody*>();
	view.each([&registry, this](entt::entity entity, VelocityComponent& velocityComp, JumpSpeedComponent jump, entt::tag<JumpedInputTag> tag, KinematicBody* pBody)
	{
		if (!pBody->is_on_floor())
			return;
		
		Update(velocityComp, jump);
		registry.remove<entt::tag<JumpedInputTag> >(entity);
	});
}
