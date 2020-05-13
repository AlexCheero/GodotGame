#include "Utils.h"

#include <string>
#include <vector>

#include <ProjectSettings.hpp>
#include <PhysicsDirectSpaceState.hpp>
#include <World.hpp>
#include <CollisionShape.hpp>
#include <CapsuleShape.hpp>

std::vector<godot::String> LayerNames;
void utils::InitPhysicLayers()
{
	const int LAYER_NAMES_MAX_COUNT = 20;
	godot::ProjectSettings* pSetts = godot::ProjectSettings::get_singleton();
	for (int64_t i = 0; i < LAYER_NAMES_MAX_COUNT; i++)
	{
		godot::String settingStr = "layer_names/3d_physics/layer_" + godot::String::num_int64(i + 1);
		if (!pSetts->has_setting(settingStr))
			return;

		LayerNames.push_back(pSetts->get_setting(settingStr));
	}
}

int64_t utils::GetLayerByName(godot::String name)
{
	if (name.length() == 0)
		return ALL_LAYERS;
	for (int64_t i = 0; i < LayerNames.size(); i++)
	{
		if (LayerNames[i] == name)
			return 1ll << i;
	}
	return 0;
}

godot::Spatial* utils::CastFromSpatial(godot::Spatial* pSpatial, godot::Vector3 direction, float distance, godot::String layerName)
{
	return CastFromSpatial(pSpatial, direction, distance, GetLayerByName(layerName));
}

godot::Spatial* utils::CastFromSpatial(godot::Spatial* pSpatial, godot::Vector3 direction, float distance, int64_t mask)
{
	godot::PhysicsDirectSpaceState* spaceState = pSpatial->get_world()->get_direct_space_state();
	godot::Transform transform = pSpatial->get_transform();
	godot::Vector3 from = transform.origin;
	godot::Vector3 to = from + direction * distance;
	godot::Dictionary rayHit = spaceState->intersect_ray(from, to, godot::Array(), mask);

	if (rayHit.empty())
		return nullptr;

	godot::Object* pObj = godot::Node::___get_from_variant(rayHit["collider"]);
	return godot::Object::cast_to<godot::Spatial>(pObj);
}

BoundsComponent utils::GetCapsuleBounds(godot::Node* pCapsuleNode)
{
	godot::CollisionShape* colShape = godot::Object::cast_to<godot::CollisionShape>(pCapsuleNode);
	godot::Ref<godot::Shape> shape = colShape->get_shape();
	godot::CapsuleShape* capsuleShape = static_cast<godot::CapsuleShape*>(shape.ptr());
	//cause capsule height is hieght of the cylinder
	float capsuleRadius = capsuleShape->get_radius();
	float boundsHeight = 2 * capsuleRadius + capsuleShape->get_height();
	float boundsWidth, boundsLength;
	boundsWidth = boundsLength = capsuleRadius * 2;
	//margin used for physics, probably should increase min distance
	return { boundsWidth, boundsHeight, boundsLength, capsuleShape->get_margin() };
}

void utils::Assert(bool assertion, const char* message, const char* file, int line)
{
	if (assertion)
		return;

	std::cerr << message << "\n" << "file: " << file << ", line: " << line << "\n";
	abort();
}

bool utils::Expired(float time, int64_t& sinceMillis)
{
	int64_t currTimeMillis = godot::OS::get_singleton()->get_ticks_msec();
	if (sinceMillis + utils::SecondsToMillis(time) > currTimeMillis)
		return false;

	sinceMillis = currTimeMillis;
	return true;
}

bool utils::Vector2Equals(godot::Vector2 a, godot::Vector2 b, float eps /*= 0.01f*/)
{
	return RealEquals(a.x, b.x, eps) && RealEquals(a.y, b.y, eps);
}
