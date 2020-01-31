#include "GrenadeNode.h"

#include <OS.hpp>
#include <SceneTree.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>

#include "../Utils/Utils.h"

#include "EntityHolderNode.h"

bool godot::GrenadeNode::CheckVisibility(Object* pTarget)
{
	Spatial* pTargetSpatial = Object::cast_to<Spatial>(pTarget);
	Vector3 targetPosition = pTargetSpatial->get_global_transform().get_origin();
	Vector3 castDirection = targetPosition - get_global_transform().get_origin();
	castDirection.normalize();
	Object* pHitResult = utils::CastFromSpatial(this, castDirection, explosionRadius);

	return pTarget == pHitResult;
}

void godot::GrenadeNode::_register_methods()
{
	register_property<GrenadeNode, float>("explosion time", &GrenadeNode::explosionTime, 0);
	register_property<GrenadeNode, float>("explosion radius", &GrenadeNode::explosionRadius, 0);
	register_property<GrenadeNode, float>("damage", &GrenadeNode::damage, 0);

	register_method((char*)"_ready", &GrenadeNode::_ready);
	register_method((char*)"_process", &GrenadeNode::_process);

	register_signal<GrenadeNode>("grenade_explodes", "hitted", GODOT_VARIANT_TYPE_OBJECT);
}

const float INTERSECT_RESULTS_NUM = 32.f;

void godot::GrenadeNode::_ready()
{
	startTime = godot::OS::get_singleton()->get_ticks_msec();

	//prepare overlap sphere params
	attackShape = Ref<SphereShape>(SphereShape::_new());
	attackShape->set_radius(explosionRadius);
	//params->set_collision_mask(utils::GetLayerByName(layerName));

	params = Ref<PhysicsShapeQueryParameters>(PhysicsShapeQueryParameters::_new());
	params->set_collide_with_areas(false);
	params->set_collide_with_bodies(true);
	params->set_shape(attackShape);
	//-----------------------------

	Node* world = get_tree()->get_current_scene();
	Array signalParams;
	signalParams.push_back(this);
	connect("grenade_explodes", world, "_on_Grenade_explosion", signalParams);
}

void godot::GrenadeNode::_process(float delta)
{
	int64_t currTime = godot::OS::get_singleton()->get_ticks_msec();
	if (startTime + utils::SecondsToMillis(explosionTime) > currTime)
		return;

	params->set_transform(get_global_transform());
	PhysicsDirectSpaceState* spaceState = get_world()->get_direct_space_state();
	Array intersects = spaceState->intersect_shape(params, INTERSECT_RESULTS_NUM);

	ASSERT(hitted.size() == 0, "grenade hitted array isn't empty");

	for (int i = 0; i < intersects.size(); i++)
	{
		Dictionary dict = intersects[i];
		Object* pObj = Node::___get_from_variant(dict["collider"]);
		EntityHolderNode* pEntitiyHolder = Object::cast_to<EntityHolderNode>(pObj);
		if (pEntitiyHolder && CheckVisibility(pObj))
			hitted.push_back(pEntitiyHolder);
	}

	//TODO: passing whole object and freeing it in the callback because of bug(?) when the signal
	//		callback Array parameter is 0 sized. Ask in all chats/communities how to solve this problem
	emit_signal("grenade_explodes", this);
	//----------

	queue_free();
}