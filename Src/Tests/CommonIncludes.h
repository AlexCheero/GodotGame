#pragma once
#include <Object.hpp> //hack to fix include error

#include "doctest.h"

//<godot bindings
#include <core/Godot.hpp>
//godot bindings>

//<misc
#include <cstdlib>
//misc>

using namespace godot;

//TODO: generate random values for tests and run tests multiple times
inline float floatRand() { return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) + rand(); }
inline float floatRandIncludeHighBound(float max) { return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max))); }
inline float floatRandIncludeHighBound(float LO, float HI) { return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO))); }
inline float floatRandExcludeHighBound(float max) { return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - CMP_EPSILON))); }
inline float floatRandExcludeHighBound(float LO, float HI) { return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - CMP_EPSILON - LO))); }