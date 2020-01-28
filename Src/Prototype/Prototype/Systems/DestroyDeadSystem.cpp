#include "DestroyDeadSystem.h"

#include <Node.hpp>

void godot::DestroyDeadSystem::operator()(float delta, entt::registry& registry)
{
	auto view = registry.view<entt::tag<DeadTag>, Node*>();
	view.less([&registry](entt::entity entity, Node* pNode)
	{
		registry.destroy(entity);
		pNode->queue_free();
	});
}
