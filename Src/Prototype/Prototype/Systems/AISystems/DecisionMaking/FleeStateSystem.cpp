#include "FleeStateSystem.h"

#include "../../../Components/AIComponents/FSMStateComponents.h"
#include "../../../Components/SimpleComponents.h"

void godot::FleeStateSystem::operator()(float delta, entt::registry& registry)
{
	auto fleeView = registry.view<entt::tag<BotTag>, entt::tag<FleeStateTag> >();
	fleeView.less([]()
	{
		//TODO: implement proper fleeing system and probably merge with FleeingSystem
	});
}