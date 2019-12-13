#include "DestroyDeadSystem.h"

#include <Node.hpp>

void godot::DestroyDeadSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<DeadComponent, Node*>().each([&registry](entt::entity entity, DeadComponent deadComp, Node* pNode)
	{
		if (registry.has<PendingDeleteComponent>(entity))
			return;

		registry.assign<PendingDeleteComponent>(entity);
		pNode->queue_free();
	});

	//TODO: use on_construct OnPendingDeleteComponentConstructed (from my test project) when compilation fixed instead
	auto viewToDelete = registry.view<PendingDeleteComponent>();
	registry.destroy(viewToDelete.begin(), viewToDelete.end());
}
