#include "HealthView.h"

void godot::HealthView::_register_methods()
{
	register_property<HealthView, float>("hp", &HealthView::hp, 0);
}