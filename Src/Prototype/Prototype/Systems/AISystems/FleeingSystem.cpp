#include "FleeingSystem.h"

#include <Spatial.hpp>

#include "../../Components/AIComponents/FSMStateComponents.h"
#include "../../Components/SimpleComponents.h"

void godot::FleeingSystem::operator()(float delta, entt::registry& registry)
{
	auto players = registry.view<entt::tag<PlayerTag>, Spatial*>(entt::exclude<entt::tag<DeadTag >>);
	if (players.size() == 0)
		return;

	auto view = registry.view<entt::tag<FleeStateTag>, VelocityComponent, SpeedComponent, Spatial*>();
	view.less([&players](VelocityComponent& velocityComp, SpeedComponent speedComp, Spatial* pSpatial)
	{
		Vector3 vectorToNearestEnemy = Vector3{ 1, 1, 1 } * std::numeric_limits<float>::infinity();
		for (auto player : players)
		{
			Spatial* pPlayerSpatial = players.get<Spatial*>(player);
			Vector3 fromPlayerVector = pSpatial->get_global_transform().get_origin() - pPlayerSpatial->get_global_transform().get_origin();
			if (fromPlayerVector.length_squared() < vectorToNearestEnemy.length_squared())
				vectorToNearestEnemy = fromPlayerVector;
		}

		vectorToNearestEnemy.y = 0;
		vectorToNearestEnemy.normalize();
		float y = velocityComp.velocity.y;
		velocityComp.velocity = vectorToNearestEnemy * speedComp.speed;
		velocityComp.velocity.y = y;
	});
}