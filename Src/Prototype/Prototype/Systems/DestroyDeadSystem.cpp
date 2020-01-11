#include "DestroyDeadSystem.h"

#include <Node.hpp>

void godot::DestroyDeadSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<entt::tag<DeadTag>, Node*>().less([&registry](entt::entity entity, Node* pNode)
	{
		if (registry.has<entt::tag<PendingDeleteTag> >(entity))
			return;

		registry.assign<entt::tag<PendingDeleteTag> >(entity);
		pNode->queue_free();
	});

	//TODO: use on_construct OnPendingDeleteComponentConstructed (from my test project) when compilation fixed instead
	auto viewToDelete = registry.view<entt::tag<PendingDeleteTag> >();
	registry.destroy(viewToDelete.begin(), viewToDelete.end());
}
