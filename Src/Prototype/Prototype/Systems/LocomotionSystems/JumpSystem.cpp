#include "JumpSystem.h"

#include <Input.hpp>

#include "../../Components/InputComponents.h"
#include "../../Components/SimpleComponents.h"

void godot::JumpSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<VelocityComponent, JumpSpeedComponent, InputComponent>(entt::exclude<InAirTag>);
	view.each([&registry](VelocityComponent& velocityComp, JumpSpeedComponent jump, InputComponent comp)
	{
		if (!comp.Test(EInput::Jump))
			return;
		
		velocityComp.velocity.y = jump.speed;
	});
}
