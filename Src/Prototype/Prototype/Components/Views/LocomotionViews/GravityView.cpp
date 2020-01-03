#include "GravityView.h"

void godot::GravityView::_register_methods()
{
	register_property<GravityView, float>("up acceleration", &GravityView::accUp, 0);
	register_property<GravityView, float>("down acceleration", &GravityView::accDown, 0);
}