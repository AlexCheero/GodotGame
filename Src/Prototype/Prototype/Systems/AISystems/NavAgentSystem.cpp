#include "NavAgentSystem.h"

#include "../../Components/Enemy.h"
#include "../../Components/SimpleComponents.h"

void godot::NavAgentSystem::operator()(float delta, entt::registry& registry)
{
	//TODO: create some kind of followingPathComponent instead of fields in Enemy
	//TODO: why use NavigationMeshInstance instead of NavigationMesh?
	registry.view<Enemy*/*, Navigation**/, VelocityComponent>().each([this, delta](Enemy* pEnemy/*, Navigation* nav*/, VelocityComponent& velocity)
	{
		//TODO: cache path
		//PoolVector3Array path = nav->get_simple_path(pEnemy->get_transform().origin, pEnemy->moveTarget);

		//if path_ind < path.size() :
		//	var move_vec = (path[path_ind] - global_transform.origin)
		//	if move_vec.length() < 0.1 :
		//		path_ind += 1
		//	else :
		//		move_and_slide(move_vec.normalized() * move_speed, Vector3(0, 1, 0))

		/*
		NavigationComponent& nav = pEnemy->navigation;
		if (nav.pathIndex < nav.path.size())
		{
			Vector3 moveVec = nav.path[nav.pathIndex] - pEnemy->get_global_transform().origin;
			if (moveVec.length() < 0.1f) //implement precision
				nav.pathIndex++;
			else
				velocity.velocity = moveVec.normalized() * nav.moveSpeed;
		}
		*/

		if (pEnemy->navigation.pathIndex < pEnemy->navigation.path.size())
		{
			Vector3 origin = pEnemy->get_global_transform().origin;
			//Godot::print("origin: " + String::num_real(origin.x) + ", " + String::num_real(origin.y) + ", " + String::num_real(origin.z));
			Vector3 moveVec = pEnemy->navigation.path[pEnemy->navigation.pathIndex] - origin;
			if (moveVec.length() < 0.1f) //implement precision
				pEnemy->navigation.pathIndex++;
			else
				velocity.velocity = moveVec.normalized() * pEnemy->navigation.moveSpeed;
		}
	});
}