#include "DestroyDeadSystem.h"

#include <Node.hpp>

void godot::DestroyDeadSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<DeadTag, Node*>().each([&registry](entt::entity entity, DeadTag deadComp, Node* pNode)
	{
		if (registry.has<PendingDeleteTag>(entity))
			return;

		registry.assign<PendingDeleteTag>(entity);
		pNode->queue_free();
	});

	//TODO: use on_construct OnPendingDeleteComponentConstructed (from my test project) when compilation fixed instead
	auto viewToDelete = registry.view<PendingDeleteTag>();
	registry.destroy(viewToDelete.begin(), viewToDelete.end());
}
