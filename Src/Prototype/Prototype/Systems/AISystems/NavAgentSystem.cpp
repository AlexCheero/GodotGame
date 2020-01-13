#include "NavAgentSystem.h"

#include <KinematicBody.hpp>

#include "../../Components/SimpleComponents.h"
#include "../../Components/AIComponents/NavigationComponents.h"
#include "../../Components/AIComponents/PatrolComponents.h"
#include "../../Components/AIComponents/FSMStateComponents.h"

//TODO: smooth nav path following with bezier
void godot::NavAgentSystem::operator()(float delta, entt::registry& registry)
{
	//TODO: audit all the systems for redundant view arguments
	auto view = registry.view<KinematicBody*, VelocityComponent, NavMarginComponent, BoundsComponent
		, SpeedComponent, NavPathComponent>(entt::exclude<entt::tag<PathFinishedTag> >);
	view.each(
	//TODO: can use auto in lambda's params declaration
	[&registry](entt::entity entity, KinematicBody* pKBody, VelocityComponent& velocity,
		NavMarginComponent marginComp, BoundsComponent bounds, SpeedComponent speedComp, NavPathComponent& pathComp)
	{
		if (!pKBody->is_on_floor())
			return;

		if (pathComp.PathComplete())
		{
			velocity.velocity.x = velocity.velocity.z = 0;
			registry.remove<NavPathComponent>(entity);
			registry.assign<entt::tag<PathFinishedTag> >(entity);
			return;
		}

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
				//TODO: prefer the get member function of a view instead of that of a registry during iterations to get the types iterated by the view itself
				registry.get<RotationDirectionComponent>(entity).direction = moveVec;
			moveVec *= speedComp.speed;
			//direct assign not to discard gravity's y influence
			velocity.velocity.x = moveVec.x;
			velocity.velocity.z = moveVec.z;
		}
	});

	auto finishedPathView = registry.view<entt::tag<PathFinishedTag> >(entt::exclude<entt::tag<PatrollingTag> >);
	finishedPathView.less([&registry](entt::entity entity)
	{
		//TODO: not necessary Patrolling after this
		registry.assign<entt::tag<PatrollingTag> >(entity);
	});
}