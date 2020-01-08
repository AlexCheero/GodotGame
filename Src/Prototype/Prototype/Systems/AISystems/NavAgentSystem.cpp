#include "NavAgentSystem.h"

#include <KinematicBody.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"

//TODO: smooth nav path following with bezier
void godot::NavAgentSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<KinematicBody*, VelocityComponent, NavAgentComponent, BoundsComponent, SpeedComponent, NavPathComponent>();
	view.each(
	[&registry](entt::entity entity, KinematicBody* pKBody, VelocityComponent& velocity,
							 NavAgentComponent navigation, BoundsComponent bounds, SpeedComponent speedComp, NavPathComponent& pathComp)
	{
		if (!pKBody->is_on_floor())
			return;

		if (!pathComp.PathComplete())
		{
			Vector3 origin = pKBody->get_global_transform().origin;
			origin.y -= bounds.height / 2;
			Vector3 moveVec = pathComp.CurrentPathPoint() - origin;

			float minDist = navigation.minDistance + bounds.width / 2;
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