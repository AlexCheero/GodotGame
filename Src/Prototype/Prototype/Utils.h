#pragma once

#include <stdint.h>

#include <Vector2.hpp>
#include <Vector3.hpp>

namespace utils
{
	void InitPhysicLayers();
	int GetLayerByName(const char* name);
	int64_t SecondsToMillis(float seconds);
	godot::Vector2 FlatVector(godot::Vector3 vec3);
}