#include "DestroyDeadSystem.h"

#include <Node.hpp>

void godot::DestroyDeadSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<DeadTag, Node*>();
	view.less([&registry](entt::entity entity, Node* pNode)
	{
		//TODO0: can't make it reactive because can't destroy in on_construct callback
		//       implement ecs events and make DeadTag an event, to make this system reactive
		registry.destroy(entity);
		pNode->queue_free();
	});
}
