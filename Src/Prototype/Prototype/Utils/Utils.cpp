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
		return 2147483647;//const for all layers
	for (int64_t i = 0; i < LayerNames.size(); i++)
	{
		if (LayerNames[i] == name)
			return 1ll << i;
	}
	return 0;
}

//TODO: find out why I can't inline this func
int64_t utils::SecondsToMillis(float seconds)
{
	return static_cast<int64_t>(static_cast<double>(seconds) * 1000);
}

godot::Vector2 utils::FlatVector(godot::Vector3 vec3)
{
	return godot::Vector2{ vec3.x, vec3.z };
}

godot::Object* utils::CastFromSpatial(godot::Spatial* pSpatial, float distance, godot::String layerName)
{
	godot::PhysicsDirectSpaceState* spaceState = pSpatial->get_world()->get_direct_space_state();
	godot::Transform attackerTransform = pSpatial->get_transform();
	godot::Vector3 from = attackerTransform.origin;
	godot::Vector3 to = from + attackerTransform.basis.z * distance;
	godot::Dictionary rayHit = spaceState->intersect_ray(from, to, godot::Array(), GetLayerByName(layerName));

	if (rayHit.empty())
		return nullptr;

	return godot::Node::___get_from_variant(rayHit["collider"]);
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
