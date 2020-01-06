#include "NavAgentSystem.h"

#include <KinematicBody.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"

void godot::NavAgentSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<KinematicBody*, VelocityComponent, NavAgentComponent, SpeedComponent, NavPathComponent>();
	view.each(
	[&registry](entt::entity entity, KinematicBody* pKBody, VelocityComponent& velocity,
							 NavAgentComponent navigation, SpeedComponent speedComp, NavPathComponent& pathComp)
	{
		if (!pKBody->is_on_floor())
			return;

		if (!pathComp.PathComplete())
		{
			Vector3 origin = pKBody->get_global_transform().origin;
			origin.y -= navigation.agentOriginHeight;
			Vector3 moveVec = pathComp.CurrentPathPoint() - origin;

			float minDist = navigation.minDistance + navigation.agentRadius;
			if (moveVec.length() < minDist)
				pathComp.pathIndex++;
			else
			{
				moveVec.normalize();
				if (registry.has<RotationDirectionComponent>(entity))
					registry.get<RotationDirectionComponent>(entity).direction = moveVec;
				moveVec *= speedComp.speed;
				//direct assign not to discard gravity's y influence
				velocity.velocity.x = moveVec.x;
				velocity.velocity.z = moveVec.z;
			}
		}
		else
		{
			velocity.velocity.x = velocity.velocity.z = 0;
			registry.remove<NavPathComponent>(entity);
		}
	});
}