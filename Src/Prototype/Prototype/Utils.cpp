#include "Utils.h"

#include <string>
#include <vector>

#include <ProjectSettings.hpp>

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

int utils::GetLayerByName(const char* name)
{
	for (int i = 0; i < LayerNames.size(); i++)
	{
		if (LayerNames[i] == name)
			return 1 << i;
	}
	return 0;
}
