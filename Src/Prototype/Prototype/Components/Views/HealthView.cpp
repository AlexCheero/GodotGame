#include "HealthView.h"

void godot::HealthView::_register_methods()
{
	register_property<HealthView, float>("hp", &HealthView::hp, 0);
	register_property<HealthView, float>("max hp", &HealthView::maxHp, 0);
}