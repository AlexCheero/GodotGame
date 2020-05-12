#include "KinematicMovementSystem.h"

#include "../../Components/SimpleComponents.h"

#include <KinematicBody.hpp>

void godot::KinematicMovementSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<VelocityComponent, KinematicBody*>().each([](VelocityComponent velocityComp, KinematicBody* pKBody)
	{
		pKBody->move_and_slide(velocityComp.velocity, Vector3(0, 1, 0));
	});

	auto onFloorCheckView = registry.view<InAirTag, VelocityComponent, KinematicBody*>();
	onFloorCheckView.less([&registry](entt::entity entity, VelocityComponent& velocityComp, KinematicBody* pKBody)
	{
		if (!pKBody->is_on_floor())
			return;

		registry.remove<InAirTag>(entity);
		velocityComp.velocity.y = 0;
	});

	auto inAirCheckView = registry.view<KinematicBody*>(entt::exclude<InAirTag>);
	inAirCheckView.each([&registry](entt::entity entity, KinematicBody* pKBody)
	{
		if (!pKBody->is_on_floor())
			registry.assign<InAirTag>(entity);
	});
}
