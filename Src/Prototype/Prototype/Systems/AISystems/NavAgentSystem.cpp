#include "NavAgentSystem.h"

#include <KinematicBody.hpp>
#include <Spatial.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"

//TODO: smooth nav path following with bezier
void godot::NavAgentSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<Spatial*, VelocityComponent, NavMarginComponent, BoundsComponent
		, SpeedComponent, NavPathComponent, RotationDirectionComponent>(entt::exclude<InAirTag>);
	view.each(
	[&registry](entt::entity entity, Spatial* pSpatial, VelocityComponent& velocity,
		NavMarginComponent marginComp, BoundsComponent bounds, SpeedComponent speedComp,
		NavPathComponent& pathComp, RotationDirectionComponent& rotDirComp)
	{
		if (pathComp.PathComplete())
		{
			velocity.velocity.x = velocity.velocity.z = 0;
			registry.remove<NavPathComponent>(entity);
			return;
		}

		Vector3 origin = pSpatial->get_global_transform().origin;
		origin.y -= bounds.height / 2;
		Vector3 moveVec = pathComp.CurrentPathPoint() - origin;

		float minDist = marginComp.margin + bounds.width / 2;
		if (moveVec.length() < minDist)
			pathComp.pathIndex++;
		else
		{
			moveVec.normalize();
			rotDirComp.direction = moveVec;
			moveVec *= speedComp.speed;
			//direct assign not to discard gravity's y influence
			velocity.velocity.x = moveVec.x;
			velocity.velocity.z = moveVec.z;
		}
	});
}