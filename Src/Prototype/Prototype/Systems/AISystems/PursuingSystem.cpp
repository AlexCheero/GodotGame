#include "PursuingSystem.h"

#include <Spatial.hpp>
#include <Navigation.hpp>

#include "../../Components/AIComponents/NavigationComponents.h"

void godot::PursuingSystem::operator()(float delta, entt::registry& registry)
{
	entt::entity navEntity = registry.view<Navigation*>()[0];
	Navigation* pNavigation = registry.get<Navigation*>(navEntity);

	auto view = registry.view <PursuingComponent, Spatial*>();
	view.each([&registry, pNavigation](entt::entity entity, PursuingComponent& comp, Spatial* pSpatial)
	{
		//TODO: assert registry.valid(comp.target);
		//TODO: assert registry.has<Spatial*>(comp.target);
		Spatial* pTargetSpatial = registry.get<Spatial*>(comp.target);
		//TODO: make nav system to target to the floor of the point or don't take target's y into account
		//TODO: only changes its path, after getting to previous target position

		Vector3 targetPosition = pTargetSpatial->get_global_transform().origin;
		Vector3 pursuerPosition = pSpatial->get_global_transform().origin;
		
		//TODO: move to PursuingView
		//TODO: change delta depending on distance to target (lower distance == lower taget)
		//		can even check angle deltas instead of distance deltas on big distances
		const float minSquaredDistanceDelta = 1.f;
		bool hasPath = registry.has<NavPathComponent>(entity);
		bool targetChangedPosition = (targetPosition - comp.previousTargetPosition).length_squared() > minSquaredDistanceDelta;
		NavPathComponent newPath;
		if (!hasPath || targetChangedPosition)
		{
			newPath.pathIndex = 0;
			newPath.path = pNavigation->get_simple_path(pursuerPosition, targetPosition);
			comp.previousTargetPosition = targetPosition;
			//TODO: try to get rid of stuff like assign_or_replace and has
			registry.assign_or_replace<NavPathComponent>(entity, newPath);
		}
	});
}