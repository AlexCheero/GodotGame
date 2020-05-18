#pragma once

#include <stdint.h>

#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Spatial.hpp>
#include <OS.hpp>

#include "../Components/SimpleComponents.h"

#ifdef DEBUG
//TODO: use doctest assert and remove #include "Utils.h" everywhere where this assert is used
//TODO: do not include this header in release build, if code uses only assertion
#define ASSERT(assertion, message) utils::Assert(assertion, message, __FILE__, __LINE__)
#else
#define ASSERT(assertion, message)
#endif

constexpr float PI = 3.1415927;

constexpr int ALL_LAYERS = 2147483647;

namespace utils
{
	const godot::Vector3 globalX = godot::Vector3{ 1, 0, 0 };
	const godot::Vector3 globalY = godot::Vector3{ 0, 1, 0 };
	const godot::Vector3 globalZ = godot::Vector3{ 0, 0, 1 };

	void InitPhysicLayers();
	//TODO: make GetMask function, with variadic params
	int64_t GetLayerByName(godot::String name);
	
	int64_t GetDamageableMask();
	inline int64_t utils::GetDamageableMask() { return GetLayerByName("Character") | GetLayerByName("Damageable"); }

	int64_t SecondsToMillis(float seconds);
	inline int64_t utils::SecondsToMillis(float seconds) { return static_cast<int64_t>(static_cast<double>(seconds) * 1000); }

	godot::Spatial* CastFromSpatial(godot::Spatial* pSpatial, godot::Vector3 direction, float distance, godot::String layerName);
	godot::Spatial* CastFromSpatial(godot::Spatial* pSpatial, godot::Vector3 direction, float distance, int64_t mask = ALL_LAYERS);
	BoundsComponent GetCapsuleBounds(godot::Node* pCapsuleNode);
	void Assert(bool assertion, const char* message, const char* file, int line);
	bool Expired(float time, int64_t& sinceMillis);
	bool Vector2Equals(godot::Vector2 a, godot::Vector2 b, float eps = 0.01f);
	godot::Vector3 GetRelativeFlatDirection(godot::Vector2 inputDir, godot::Vector3 left, godot::Vector3 forward);

	float Deg2rad(float p_y);
	inline float Deg2rad(float p_y) { return p_y * PI / 180.0; }
	float Rad2deg(float p_y);
	inline float Rad2deg(float p_y) { return p_y * 180.0 / PI; }
	
	template<typename T>
	T* GetParentOfType(godot::Node* pNode);
	template<typename T>
	bool RealEquals(T a, T b, T eps = std::numeric_limits<T>::epsilon());
}

template<typename T>
T* utils::GetParentOfType(godot::Node* pNode)
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

template<typename T>
bool utils::RealEquals(T a, T b, T eps/* = std::numeric_limits<T>::epsilon()*/)
{
	return std::abs(a - b) <= eps;
}
