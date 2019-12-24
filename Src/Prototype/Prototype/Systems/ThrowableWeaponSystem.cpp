#include "ThrowableWeaponSystem.h"

#include "../Components/Throwable.h"
#include "../Components/SimpleComponents.h"

void godot::ThrowableWeaponSystem::operator()(float delta, entt::registry& registry)
{
	registry.view<Throwable*>().each([&registry](entt::entity throwableEntity, Throwable* pThrowable)
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
		//hittedHealthComp.hp -= damage;
		hittedHealthComp.hp = 0;

		if (hittedHealthComp.hp <= 0)
		{
			Godot::print("Kill!");
			hittedHealthComp.hp = 0;
			registry.assign<entt::tag<DeadTag> >(hittedEntity);
		}
		else
			Godot::print("Hit!");
	});
}