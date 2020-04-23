#pragma once

#include <stdint.h>

#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Spatial.hpp>

#include "../Components/SimpleComponents.h"

#ifdef DEBUG
//TODO: use doctest assert and remove #include "Utils.h" everywhere where this assert is used
//TODO: do not include this header in release build, if code uses only assertion
#define ASSERT(assertion, message) utils::Assert(assertion, message, __FILE__, __LINE__)
#else
#define ASSERT(assertion, message)
#endif

namespace utils
{
	const godot::Vector3 globalX = godot::Vector3{ 1, 0, 0 };
	const godot::Vector3 globalY = godot::Vector3{ 0, 1, 0 };
	const godot::Vector3 globalZ = godot::Vector3{ 0, 0, 1 };

	void InitPhysicLayers();
	int64_t GetLayerByName(godot::String name);
	int64_t SecondsToMillis(float seconds);
	godot::Vector2 FlatVector(godot::Vector3 vec3);
	godot::Object* CastFromSpatial(godot::Spatial* pSpatial, godot::Vector3 direction, float distance, godot::String layerName = "");
	BoundsComponent GetCapsuleBounds(godot::Node* pCapsuleNode);
	void Assert(bool assertion, const char* message, const char* file, int line);
	
	template<typename T>
	T* GetParentOfType(godot::Node* pNode)
	{
		godot::Node* pParentNode = pNode->get_parent();
		if (!pParentNode)
			return nullptr;

		T* castedParent = godot::Object::cast_to<T>(pParentNode);
		if (castedParent)
			return castedParent;
		else
			return GetParentOfType<T>(pParentNode);
	}
}