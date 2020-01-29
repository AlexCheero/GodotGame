#include "ThrowableWeaponSystem.h"

#include "../../Components/NodeComponents/ThrowableNodeComponent.h"
#include "../../Components/SimpleComponents.h"

void godot::ThrowableWeaponSystem::operator()(float delta, entt::registry& registry)
{
	return;
	auto view = registry.view<ThrowableNodeComponent*>(ExcludeDead);
	view.each([&registry](entt::entity throwableEntity, ThrowableNodeComponent* pThrowable)
	{
		if (pThrowable->is_contact_monitor_enabled())
			return;
		
		Node* throwableNode = Object::cast_to<Node>(pThrowable);
		registry.assign<Node*>(throwableEntity, throwableNode);
		registry.assign<entt::tag<DeadTag> >(throwableEntity);

		entt::entity hittedEntity = pThrowable->GetHittedEntity();
		if (!registry.valid(hittedEntity))
			return;

		HealthComponent& hittedHealthComp = registry.get<HealthComponent>(hittedEntity);

		hittedHealthComp.hp -= pThrowable->GetDamage();
	});
}