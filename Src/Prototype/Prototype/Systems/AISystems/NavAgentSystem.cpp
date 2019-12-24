#include "NavAgentSystem.h"

//#include <NavigationMeshInstance.hpp>
//#include <Ref.hpp>
//#include <NavigationMesh.hpp>
#include <Navigation.hpp>
#include <PoolArrays.hpp>

#include "../../Components/Enemy.h"

void godot::NavAgentSystem::operator()(float delta, entt::registry& registry)
{
	//TODO: create some kind of followingPathComponent instead of fields in Enemy
	//TODO: why use NavigationMeshInstance instead of NavigationMesh?
	registry.view<Enemy*, Navigation*>().each([this, delta](Enemy* pEnemy, Navigation* nav)
	{
		//TODO: cache path
		PoolVector3Array path = nav->get_simple_path(pEnemy->get_transform().origin, pEnemy->moveTarget);
	});
}