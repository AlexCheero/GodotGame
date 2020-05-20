#include "EndAttackAnimSystem.h"

#include "../../Components/AttackComponents.h"

void godot::EndAttackAnimSystem::Tick(float delta, entt::registry& registry)
{
	auto view = registry.view<AttackAnimPlayingComponent>();
	view.each([&registry, delta](entt::entity entity, AttackAnimPlayingComponent& attackPlayingComp)
	{
		attackPlayingComp.playBackTimeLeft -= delta;

		if (attackPlayingComp.playBackTimeLeft <= 0)
			registry.remove<AttackAnimPlayingComponent>(entity);
	});
}