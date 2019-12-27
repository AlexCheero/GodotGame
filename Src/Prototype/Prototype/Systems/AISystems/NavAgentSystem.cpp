#include "NavAgentSystem.h"

#include "../../Components/Enemy.h"
#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"

void godot::NavAgentSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<Enemy*, VelocityComponent, NavAgentComponent, SpeedComponent, NavPathComponent>();
	view.each(
	[&registry, this, delta](entt::entity entity, Enemy* pEnemy, VelocityComponent& velocity,
							 NavAgentComponent& navigation, SpeedComponent speedComp, NavPathComponent pathComp)
	{
		if (!pEnemy->is_on_floor())
			return;

		if (navigation.prevPathIndex != navigation.pathIndex)
		{
		//	Godot::print("path idx: " + String::num_int64(navigation.pathIndex) + ", total: " + String::num_int64(pathComp.path.size()));
			navigation.prevPathIndex = navigation.pathIndex;
		}

		if (navigation.pathIndex < pathComp.path.size())
		{
			Vector3 origin = pEnemy->get_global_transform().origin;
			origin.y -= navigation.agentOriginHeight;
			Vector3 currTarget = pathComp.path[navigation.pathIndex];
			Vector3 moveVec = currTarget - origin;

			//TODO: implement precision/minDistance
			//TODO: take agent height and width into account
			float minDist = 0.1f;//navigation.minDistance + navigation.agentRadius
			if (moveVec.length() < minDist)
			{
				navigation.pathIndex++;
			}
			else
			{
				//TODO: don't discard gravity's y influence
				float y = velocity.velocity.y;
				velocity.velocity = moveVec.normalized() * speedComp.speed;
				velocity.velocity.y += y;
			}
		}
		else
			registry.remove<NavPathComponent>(entity);
	});
}