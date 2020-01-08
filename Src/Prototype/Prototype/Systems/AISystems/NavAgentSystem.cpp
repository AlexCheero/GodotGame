#include "NavAgentSystem.h"

#include <KinematicBody.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"
#include "../../Components/AIComponents/PatrolComponents.h"

//TODO: smooth nav path following with bezier
void godot::NavAgentSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<KinematicBody*, VelocityComponent, NavMarginComponent, BoundsComponent, SpeedComponent, NavPathComponent>();
	view.each(
	[&registry](entt::entity entity, KinematicBody* pKBody, VelocityComponent& velocity,
		NavMarginComponent marginComp, BoundsComponent bounds, SpeedComponent speedComp, NavPathComponent& pathComp)
	{
		if (!pKBody->is_on_floor())
			return;

		if (!pathComp.PathComplete())
		{
			Vector3 origin = pKBody->get_global_transform().origin;
			origin.y -= bounds.height / 2;
			Vector3 moveVec = pathComp.CurrentPathPoint() - origin;

			float minDist = marginComp.margin + bounds.width / 2;
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

			//TODO: not working properly after cathcing up player
			//see player second time only on patrol points
			if (!registry.has<entt::tag<PatrollingTag> >(entity))
				registry.assign<entt::tag<PatrollingTag> >(entity);
		}
	});
}