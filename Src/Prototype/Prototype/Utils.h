#pragma once

#include <stdint.h>

namespace utils
{
	void InitPhysicLayers();
	int GetLayerByName(const char* name);
	int64_t SecondsToMillis(float seconds);
}