#include "HTHDamagingArea.h"

#include <SceneTree.hpp>

void godot::HTHDamagingArea::_register_methods()
{
	register_method((char*)"_on_Area_body_entered", &HTHDamagingArea::_on_Area_body_entered);
}

void godot::HTHDamagingArea::_on_Area_body_entered(EntityHolderNode* pEntityHolder)
{
	if (!pEntityHolder)
		return;
}