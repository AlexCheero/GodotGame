#include "HealthMonitoringSystem.h"

#include "../../Components/SimpleComponents.h"

void godot::HealthMonitoringSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<HealthComponent>();
	view.each([&registry](entt::entity entity, HealthComponent& healthComp)
	{
		if (healthComp.hp > 0)
			return;
		
		Godot::print("Died!");
		healthComp.hp = 0;
		registry.assign<entt::tag<DeadTag> >(entity);
	});
}