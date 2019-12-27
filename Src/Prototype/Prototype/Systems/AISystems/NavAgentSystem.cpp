#include "NavAgentSystem.h"

#include "../../Components/Enemy.h"
#include "../../Components/SimpleComponents.h"

void godot::NavAgentSystem::operator()(float delta, entt::registry& registry)
{
	//TODO: create some kind of followingPathComponent instead of fields in Enemy
	registry.view<Enemy*, VelocityComponent>().each([this, delta](Enemy* pEnemy, VelocityComponent& velocity)
	{
		if (!pEnemy->is_on_floor())
			return;

		if (pEnemy->navigation.pathIndex < pEnemy->navigation.path.size())
		{
			Vector3 origin = pEnemy->get_global_transform().origin;
			Vector3 currTarget = pEnemy->navigation.path[pEnemy->navigation.pathIndex];
			Vector3 moveVec = currTarget - origin;

			//TODO: implement precision
			//TODO: take agent height into account
			float precision = 1.2f;//0.1f;
			if (moveVec.length() < precision)
			{
				pEnemy->navigation.pathIndex++;
			}
			else
			{
				//TODO: don't discard gravity's y influence
				float y = velocity.velocity.y;
				velocity.velocity = moveVec.normalized() * pEnemy->navigation.moveSpeed;
				velocity.velocity.y += y;
			}
		}
	});
}