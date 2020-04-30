#include "GrenadeSystem.h"

#include <OS.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "../../Components/AttackComponents.h"
#include "../../Components/SimpleComponents.h"
#include "../../Nodes/EntityView.h"

const float INTERSECT_RESULTS_NUM = 32.f;

bool godot::GrenadeSystem::CheckVisibility(Spatial* pGrenade, Spatial* pTarget, float explosionRadius)
{
	Vector3 castDirection = pTarget->get_global_transform().get_origin() - pGrenade->get_global_transform().get_origin();
	castDirection.normalize();

	return pTarget == utils::CastFromSpatial(pGrenade, castDirection, explosionRadius);
}

godot::GrenadeSystem::GrenadeSystem()
{
	m_params = Ref<PhysicsShapeQueryParameters>(PhysicsShapeQueryParameters::_new());
	m_params->set_collide_with_areas(false);
	m_params->set_collide_with_bodies(true);

	m_attackShape = Ref<SphereShape>(SphereShape::_new());
	m_params->set_shape(m_attackShape);
	m_params->set_collision_mask(utils::GetDamageableMask());
}

void godot::GrenadeSystem::operator()(float delta, entt::registry& registry)
{
	int64_t currTime = godot::OS::get_singleton()->get_ticks_msec();

	auto view = registry.view<GrenadeComponent, Spatial*>(entt::exclude<GrenadeExplodesTag>);
	view.each([&registry, currTime](entt::entity entity, GrenadeComponent grenComp, Spatial* pGrenSpatial)
	{
		if (grenComp.startTime + utils::SecondsToMillis(grenComp.explosionTime) <= currTime)
			registry.assign<GrenadeExplodesTag>(entity);
	});

	auto explodedView = registry.view<GrenadeExplodesTag, GrenadeComponent, Spatial*>(ExcludeDead);
	explodedView.less([this, &registry](entt::entity entity, GrenadeComponent grenComp, Spatial* pGrenSpatial)
	{
		m_attackShape->set_radius(grenComp.explosionRadius);
		m_params->set_transform(pGrenSpatial->get_global_transform());
		PhysicsDirectSpaceState* spaceState = pGrenSpatial->get_world()->get_direct_space_state();
		Array intersects = spaceState->intersect_shape(m_params, INTERSECT_RESULTS_NUM);

		for (int i = 0; i < intersects.size(); i++)
		{
			Dictionary dict = intersects[i];
			Object* pObj = Node::___get_from_variant(dict["collider"]);
			Spatial* pTargetSpatial = Object::cast_to<Spatial>(pObj);

			if (pTargetSpatial && pTargetSpatial->has_node("EntityView") && CheckVisibility(pGrenSpatial, pTargetSpatial, grenComp.explosionRadius))
			{
				EntityView* pEntityView = Object::cast_to<EntityView>(pTargetSpatial->get_node("EntityView"));
				//because is_instance_valid is not presented in the bindings
				if (!registry.valid(pEntityView->GetEntity()))
					return;

				//TODO: check teammate if friendly fire is off

				ASSERT(registry.has<HealthComponent>(pEntityView->GetEntity()), "no health component on entity hitted by explosion");
				registry.get<HealthComponent>(pEntityView->GetEntity()).hp -= grenComp.damage;
			}
		}

		registry.assign<DeadTag>(entity);
		registry.assign<Node*>(entity, Object::cast_to<Node>(pGrenSpatial));
	});
}