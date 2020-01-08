#include "NavMarginView.h"

void godot::NavMarginView::_register_methods()
{
	register_property<NavMarginView, float>("min navigation margin", &NavMarginView::margin, 0);
}