#pragma once

#include <stdint.h>

#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Spatial.hpp>

namespace utils
{
	const godot::Vector3 globalX = godot::Vector3{ 1, 0, 0 };
	const godot::Vector3 globalY = godot::Vector3{ 0, 1, 0 };
	const godot::Vector3 globalZ = godot::Vector3{ 0, 0, 1 };

	void InitPhysicLayers();
	int64_t GetLayerByName(godot::String name);
	int64_t SecondsToMillis(float seconds);
	godot::Vector2 FlatVector(godot::Vector3 vec3);
	godot::Object* CastFromSpatial(godot::Spatial* pSpatial, float distance, godot::String layerName);
}