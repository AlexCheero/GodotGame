#include "HTHDamagingArea.h"

#include <SceneTree.hpp>

void godot::HTHDamagingArea::_register_methods()
{
	register_method((char*)"_on_Area_body_entered", &HTHDamagingArea::_on_Area_body_entered);

	register_signal<HTHDamagingArea>("hth_hit", "hitted_entity", GODOT_VARIANT_TYPE_INT);
}

void godot::HTHDamagingArea::_on_Area_body_entered(EntityHolderNode* pEntityHolder)
{
	if (!pEntityHolder)
		return;

	emit_signal("hth_hit", (int)pEntityHolder->GetEntity());
}