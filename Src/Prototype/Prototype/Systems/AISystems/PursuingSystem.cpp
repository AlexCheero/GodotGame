#include "PursuingSystem.h"

#include <Spatial.hpp>
#include <Navigation.hpp>

#include "../../Components/AIComponents/NavigationComponents.h"
#include "../../Components/AIComponents/FSMStateComponents.h"

void godot::PursuingSystem::operator()(float delta, entt::registry& registry)
{
	entt::entity navEntity = registry.view<Navigation*>()[0];
	Navigation* pNavigation = registry.get<Navigation*>(navEntity);

	auto view = registry.view<PursuingStateComponent, DecisionMakingComponent, Spatial*>();
	view.each([&registry, pNavigation](entt::entity entity, PursuingStateComponent& comp, DecisionMakingComponent decision, Spatial* pSpatial)
	{
		//TODO: change delta depending on distance to target (lower distance == lower taget)
		//		can even check angle deltas instead of distance deltas on big distances
		bool hasPath = registry.has<NavPathComponent>(entity);
		Spatial* pTargetSpatial = registry.get<Spatial*>(comp.target);
		Vector3 targetPosition = pTargetSpatial->get_global_transform().origin;
		
		bool targetChangedPosition = (targetPosition - comp.previousTargetPosition).length_squared() > decision.targetPosRecalcDistance;
		if (!hasPath || targetChangedPosition)
		{
			NavPathComponent newPath;
			newPath.pathIndex = 0;
			Vector3 pursuerPosition = pSpatial->get_global_transform().origin;
			newPath.path = pNavigation->get_simple_path(pursuerPosition, targetPosition);
			comp.previousTargetPosition = targetPosition;
			registry.emplace_or_replace<NavPathComponent>(entity, newPath);
		}
	});
}