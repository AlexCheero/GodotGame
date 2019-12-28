#include "NavAgentSystem.h"

#include "../../Components/Enemy.h"
#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"

void godot::NavAgentSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<Enemy*, VelocityComponent, NavAgentComponent, SpeedComponent, NavPathComponent>();
	view.each(
	[&registry, this, delta](entt::entity entity, Enemy* pEnemy, VelocityComponent& velocity,
							 NavAgentComponent navigation, SpeedComponent speedComp, NavPathComponent& pathComp)
	{
		if (!pEnemy->is_on_floor())
			return;

		if (!pathComp.PathComplete())
		{
			Vector3 origin = pEnemy->get_global_transform().origin;
			origin.y -= navigation.agentOriginHeight;
			Vector3 moveVec = pathComp.CurrentPathPoint() - origin;

			float minDist = navigation.minDistance + navigation.agentRadius;
			if (moveVec.length() < minDist)
				pathComp.pathIndex++;
			else
			{
				moveVec.normalize();
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