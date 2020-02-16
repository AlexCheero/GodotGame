#include "GrenadeSystem.h"

#include <OS.hpp>
#include <SphereShape.hpp>
#include <PhysicsShapeQueryParameters.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "../../Components/AttackComponents.h"
#include "../../Components/SimpleComponents.h"
#include "../../Nodes/EntityHolderNode.h"

const float INTERSECT_RESULTS_NUM = 32.f;

bool godot::GrenadeSystem::CheckVisibility(Spatial* pGrenade, EntityHolderNode* pTarget, float explosionRadius)
{
	Vector3 castDirection = pTarget->get_global_transform().get_origin() - pGrenade->get_global_transform().get_origin();
	castDirection.normalize();
	Object* pHitResult = utils::CastFromSpatial(pGrenade, castDirection, explosionRadius);

	return pTarget == Object::cast_to<EntityHolderNode>(pHitResult);
}

void godot::GrenadeSystem::operator()(float delta, entt::registry& registry)
{
	int64_t currTime = godot::OS::get_singleton()->get_ticks_msec();

	auto view = registry.view<GrenadeComponent, Spatial*>(entt::exclude<entt::tag<GrenadeExplodesTag> >);
	view.each([&registry, currTime](entt::entity entity, GrenadeComponent grenComp, Spatial* pGrenSpatial)
	{
		if (grenComp.startTime + utils::SecondsToMillis(grenComp.explosionTime) <= currTime)
			registry.assign<entt::tag<GrenadeExplodesTag> >(entity);
	});

	auto explodedView = registry.view<entt::tag<GrenadeExplodesTag>, GrenadeComponent, Spatial*>(entt::exclude<entt::tag<DeadTag> >);
	explodedView.less([this, &registry](entt::entity entity, GrenadeComponent grenComp, Spatial* pGrenSpatial)
	{
		//TODO0: cache params
		Ref<SphereShape> attackShape = Ref<SphereShape>(SphereShape::_new());
		attackShape->set_radius(grenComp.explosionRadius);
		
		//TODO: set layer name
		//params->set_collision_mask(utils::GetLayerByName(layerName));
		Ref<PhysicsShapeQueryParameters> params = Ref<PhysicsShapeQueryParameters>(PhysicsShapeQueryParameters::_new());
		params->set_collide_with_areas(false);
		params->set_collide_with_bodies(true);
		params->set_shape(attackShape);

		params->set_transform(pGrenSpatial->get_global_transform());
		PhysicsDirectSpaceState* spaceState = pGrenSpatial->get_world()->get_direct_space_state();
		Array intersects = spaceState->intersect_shape(params, INTERSECT_RESULTS_NUM);

		for (int i = 0; i < intersects.size(); i++)
		{
			Dictionary dict = intersects[i];
			Object* pObj = Node::___get_from_variant(dict["collider"]);
			EntityHolderNode* pEntitiyHolder = Object::cast_to<EntityHolderNode>(pObj);

			if (pEntitiyHolder && CheckVisibility(pGrenSpatial, pEntitiyHolder, grenComp.explosionRadius))
			{
				ASSERT(pEntitiyHolder->GetEntity() != entt::null, "wrong entity, hitted by explosion");
				ASSERT(registry.has<HealthComponent>(pEntitiyHolder->GetEntity()), "no health component on entity hitted by explosion");
				registry.get<HealthComponent>(pEntitiyHolder->GetEntity()).hp -= grenComp.damage;
			}
		}

		registry.assign<entt::tag<DeadTag> >(entity);
		registry.assign<Node*>(entity, Object::cast_to<Node>(pGrenSpatial));
	});
}