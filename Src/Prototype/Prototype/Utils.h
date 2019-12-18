#pragma once

#include <stdint.h>

#include <Vector2.hpp>
#include <Vector3.hpp>

namespace utils
{
	const godot::Vector3 globalX = godot::Vector3{ 1, 0, 0 };
	const godot::Vector3 globalY = godot::Vector3{ 0, 1, 0 };
	const godot::Vector3 globalZ = godot::Vector3{ 0, 0, 1 };

	void InitPhysicLayers();
	int GetLayerByName(const char* name);
	int64_t SecondsToMillis(float seconds);
	godot::Vector2 FlatVector(godot::Vector3 vec3);
}