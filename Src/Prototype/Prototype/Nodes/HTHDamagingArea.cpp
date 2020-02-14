#include "HTHDamagingArea.h"

#include <SceneTree.hpp>

void godot::HTHDamagingArea::_register_methods()
{
	register_method((char*)"_ready", &HTHDamagingArea::_ready);
	register_method((char*)"_on_Area_body_entered", &HTHDamagingArea::_on_Area_body_entered);

	register_signal<HTHDamagingArea>("hth_hit", "hitted_entity", GODOT_VARIANT_TYPE_INT);
}

void godot::HTHDamagingArea::_ready()
{
	Node* world = get_tree()->get_current_scene();
	Array params;
	params.push_back(this);
	connect("hth_hit", world, "_on_HTH_hit", params);
}

void godot::HTHDamagingArea::_on_Area_body_entered(EntityHolderNode* pEntityHolder)
{
	emit_signal("hth_hit", (int)pEntityHolder->GetEntity());
}