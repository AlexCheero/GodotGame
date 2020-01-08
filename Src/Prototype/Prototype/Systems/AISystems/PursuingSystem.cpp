#include "PursuingSystem.h"

#include <Spatial.hpp>
#include <Navigation.hpp>

#include "../../Components/AIComponents/NavigationComponents.h"

void godot::PursuingSystem::operator()(float delta, entt::registry& registry)
{
	entt::entity navEntity = registry.view<Navigation*>()[0];
	Navigation* pNavigation = registry.get<Navigation*>(navEntity);

	auto view = registry.view <PursuingComponent, Spatial*>();
	view.each([&registry, pNavigation](entt::entity entity, PursuingComponent comp, Spatial* pSpatial)
	{
		//TODO: assert registry.has<Spatial*>(comp.target);
		Spatial* pTargetSpatial = registry.get<Spatial*>(comp.target);
		NavPathComponent& newPath = registry.assign_or_replace<NavPathComponent>(entity);
		newPath.pathIndex = 0;
		//TODO: make nav system to target to the floor of the point or don't take target's y into account
		newPath.path = pNavigation->get_simple_path(pSpatial->get_global_transform().origin, pTargetSpatial->get_global_transform().origin);
		registry.remove<PursuingComponent>(entity);
	});
}