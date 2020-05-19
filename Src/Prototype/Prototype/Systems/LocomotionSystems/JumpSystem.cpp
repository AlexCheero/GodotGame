#include "JumpSystem.h"

#include "../../Components/InputComponents.h"
#include "../../Components/SimpleComponents.h"
#include "../../Utils/Utils.h"

void godot::JumpSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<JumpPressedTag, VelocityComponent, JumpSpeedComponent>(entt::exclude<InAirTag>);
	view.each([](VelocityComponent& velComp, JumpSpeedComponent jumpComp)
	{
		velComp.velocity.y = jumpComp.speed;
	});
}
