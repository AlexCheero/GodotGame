#include "FleeStateSystem.h"

#include "../../../Components/AIComponents/FSMStateComponents.h"
#include "../../../Components/SimpleComponents.h"

void godot::FleeStateSystem::operator()(float delta, entt::registry& registry)
{
	auto fleeView = registry.view<BotTag, FleeStateTag>();
	fleeView.each([]()
	{
		//TODO: implement proper fleeing system and probably merge with FleeingSystem
	});
}